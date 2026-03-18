#include "sys/AboutSystem.hpp"

#include <cctype>
#include <dirent.h>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/stat.h>

#include "sys/utils/File.hpp"
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

  auto Dir = utils::Directory("/sys/class/dmi/id/");
  if (!Dir.isOpen()) {
    return Rows;
  }

  Dir.forEachChild([&Rows](std::string_view Filename) -> void {
    if (Filename == "." || Filename == "..") {
      return;
    }

    auto Path = "/sys/class/dmi/id/" + std::string(Filename);
    auto File = utils::File(std::move(Path));

    if (!File.isRegular()) {
      return;
    }

    auto TitleCase = snakeToTitleCase(Filename);

    auto Value = File.read();
    if (!Value.has_value()) {
      Rows.emplace_back(std::make_tuple(std::move(TitleCase)));
      return;
    }

    Rows.emplace_back(
      std::make_tuple(std::move(TitleCase), std::move(Value.value())));
  });

  return Rows;
}

} // namespace ipm::sys
