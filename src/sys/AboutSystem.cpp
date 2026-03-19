#include "sys/AboutSystem.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::sys {

auto AboutSystem::rows() -> ui::pages::Rows {
  auto Rows = ui::pages::Rows();

  auto Dir = utils::Directory("/sys/class/dmi/id/");
  if (!Dir.isOpen()) {
    return Rows;
  }

  Dir.forEachEntry([&Rows](std::string_view Filename) -> void {
    if (Filename == "." || Filename == "..") {
      return;
    }

    auto Path = "/sys/class/dmi/id/" + std::string(Filename);
    auto File = utils::File(std::move(Path));

    if (!File.isRegular()) {
      return;
    }

    auto TitleCase = File.getFieldName();

    auto Value = File.read();
    if (!Value.has_value()) {
      Rows.emplace_back(
        ui::pages::Row{ .Label = std::move(TitleCase), .Value = std::nullopt });
      return;
    }

    Rows.emplace_back(ui::pages::Row{
      .Label = std::move(TitleCase), .Value = std::move(Value.value()) });
  });

  return Rows;
}

} // namespace ipm::sys
