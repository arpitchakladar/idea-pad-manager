#include "ui/pages/PowerInformation.hpp"

#include <optional>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/animations/Battery.hpp"
#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"
#include "ui/utils/LabelButton.hpp"

namespace ipm::ui::pages {

PowerInformation::PowerInformation() {
  auto ConservationModeButton = utils::LabelButton::create("OFF",
                                  [&]() -> std::string {
                                    m_ConservationMode = !m_ConservationMode;
                                    return m_ConservationMode ? "ON " : "OFF";
                                  }) |
    ftxui::center;

  m_BatteryAnimation.resize(k_CanvasSize);

  createPage(
    { Row{ .Label = "Battery Model Name sdfasdfasfsdfasdf",
        .Value = std::nullopt },
      Row{ .Label = "Battery Technology", .Value = std::string("Li-poly") },
      Row{ .Label = "Battery Capacity",
        .Value = ([Value = ("76%")]() -> std::optional<std::string> {
          return Value;
        }) },
      Row{ .Label = "Max battery Capacity", .Value = std::string("59Wh") },
      Row{ .Label = "Manufactured Capacity", .Value = std::string("60Wh") },
      Row{ .Label = "Battery charge cycles", .Value = std::string("11") },
      Row{ .Label = "Battery Capacity",
        .Value = ([Value = ("76%")]() -> std::optional<std::string> {
          return Value;
        }) },
      Row{ .Label = "Conservation mode", .Value = ConservationModeButton } },
    "POWER INFORMATION",
    m_BatteryAnimation.canvasUpdatesPerSecond(),
    [&]() { m_BatteryAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_BatteryAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
