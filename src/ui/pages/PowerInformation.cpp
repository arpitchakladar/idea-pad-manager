#include "ui/pages/PowerInformation.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

#include "ui/animations/Battery.hpp"
#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {

PowerInformation::PowerInformation() {
  auto ConservationModeButtonOption = ftxui::ButtonOption::Simple();
  ConservationModeButtonOption.label = &m_ConservationModeButtonMessage;
  ConservationModeButtonOption.on_click = [&]() -> void {
    m_ConservationMode = !m_ConservationMode;
    m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  };

  auto ConservationModeButton =
    ftxui::Button(ConservationModeButtonOption) | ftxui::center;

  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  m_BatteryAnimation.resize(k_CanvasDimentions);

  createPage(
    { RowStatic{ "Battery Model Name sdfasdfasfsdfasdf", "L24N4P3" },
      RowStatic{ "Battery Technology", "Li-poly" },
      RowDynamic{ "Battery Capacity",
        [Value = std::make_shared<std::string>(
           "76%")]() -> std::shared_ptr<std::string> { return Value; } },
      RowStatic{ "Max battery Capacity", "59Wh" },
      RowStatic{ "Manufactured Capacity", "60Wh" },
      RowStatic{ "Battery charge cycles", "11" },
      RowDynamic{ "Battery Capacity",
        [Value = std::make_shared<std::string>(
           "76%")]() -> std::shared_ptr<std::string> { return Value; } },
      RowCustom{ "Conservation mode", ConservationModeButton } },
    "POWER INFORMATION",
    m_BatteryAnimation.canvasUpdatesPerSecond(),
    [&]() { m_BatteryAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_BatteryAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
