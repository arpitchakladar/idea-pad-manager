#include "sys/AboutSystem.hpp"

#include <cctype>
#include <dirent.h>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/stat.h>

#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::sys {
namespace {

auto snakeToTitleCase(std::string_view Snake) -> std::string {
  std::ostringstream Result;
  auto CapitalizeNext = true;

  for (unsigned char const C : Snake) {
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

} // namespace

auto AboutSystem::aboutSystemInfo()
  -> std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom,
    ui::pages::RowStaticError>> {
  auto Rows = std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom,
    ui::pages::RowStaticError>>();

  const auto DirHandle = utils::DirectoryHandle("/sys/class/dmi/id/");
  if (!DirHandle.isOpen()) {
    return Rows;
  }

  struct dirent *Entry = nullptr;
  while ((Entry = readdir(DirHandle.get())) != nullptr) {
    const auto Filename = std::string_view(&Entry->d_name[0]);

    if (Filename == "." || Filename == "..") {
      continue;
    }

    std::string const Path = "/sys/class/dmi/id/" + std::string(Filename);

    if (!utils::FileSystem::isRegularFile(Path)) {
      continue;
    }

    auto TitleCase = snakeToTitleCase(Filename);

    auto Value = utils::FileSystem::readFile(Path);
    if (!Value.has_value()) {
      Rows.emplace_back(std::make_tuple(std::move(TitleCase)));
      continue;
    }

    Rows.emplace_back(
      std::make_tuple(std::move(TitleCase), std::move(Value.value())));
  }

  return Rows;
}

} // namespace ipm::sys
