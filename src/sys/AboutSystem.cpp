#include "sys/AboutSystem.hpp"
#include "ui/pages/Page.hpp"

#include <cctype>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace ipm::sys {
namespace {
auto snakeToTitleCase(const std::string &Snake) -> std::string {
  std::ostringstream Result;
  bool CapitalizeNext = true;
  for (char const C : Snake) {
    if (C == '_') {
      CapitalizeNext = true;
      Result << ' ';
    } else if (CapitalizeNext) {
      Result << static_cast<char>(std::toupper(C));
      CapitalizeNext = false;
    } else {
      Result << C;
    }
  }
  return Result.str();
}

auto readFile(const std::string &Path) -> std::string {
  std::ifstream File(Path);
  if (!File.is_open()) {
    return "";
  }
  std::ostringstream Content;
  Content << File.rdbuf();
  std::string Value = Content.str();
  if (!Value.empty() && Value.back() == '\n') {
    Value.pop_back();
  }
  return Value;
}
} // namespace

auto AboutSystem::aboutSystemInfo()
  -> std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom>> {

  std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom>>
    Rows;

  DIR *Dir = opendir("/sys/class/dmi/id/");
  if (Dir == nullptr) {
    return Rows;
  }

  struct dirent *Entry = nullptr;
  while ((Entry = readdir(Dir)) != nullptr) {
    std::string const Filename = Entry->d_name;
    if (Filename == "." || Filename == "..") {
      continue;
    }

    std::string const Path = "/sys/class/dmi/id/" + Filename;
    std::string const Value = readFile(Path);
    std::string const TitleCase = snakeToTitleCase(Filename);

    Rows.emplace_back(std::make_tuple(TitleCase, Value));
  }

  closedir(Dir);
  return Rows;
}
} // namespace ipm::sys
