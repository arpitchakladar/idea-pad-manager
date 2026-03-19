#include "ui/pages/PowerInformation.hpp"

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
    { RowStatic{ "Battery Model Name sdfasdfasfsdfasdf", "L24N4P3" },
      RowStatic{ "Battery Technology", "Li-poly" },
      RowDynamic{ "Battery Capacity",
        [Value = ("76%")]() -> std::string { return Value; } },
      RowStatic{ "Max battery Capacity", "59Wh" },
      RowStatic{ "Manufactured Capacity", "60Wh" },
      RowStatic{ "Battery charge cycles", "11" },
      RowDynamic{ "Battery Capacity",
        [Value = ("76%")]() -> std::string { return Value; } },
      RowCustom{ "Conservation mode", ConservationModeButton } },
    "POWER INFORMATION",
    m_BatteryAnimation.canvasUpdatesPerSecond(),
    [&]() { m_BatteryAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_BatteryAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
