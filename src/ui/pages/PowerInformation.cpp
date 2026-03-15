#include "ui/pages/PowerInformation.hpp"

#include <numbers>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

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
  static constexpr auto k_FanAnimationFPS = 0U;

  createPage(
    { RowDynamic{ "Fan Speed", []() -> const char * { return "2100 RPM"; } },
      RowStatic{ "Battery Model Name", "L24N4P3" },
      RowStatic{ "Battery Technology", "Li-poly" },
      RowDynamic{ "Battery Capacity", []() -> const char * { return "76%"; } },
      RowStatic{ "Max battery Capacity", "59Wh" },
      RowStatic{ "Manufactured Capacity", "60Wh" },
      RowStatic{ "Battery charge cycles", "11" },
      RowDynamic{
        "Battery status", []() -> const char * { return "Not charging"; } },
      RowCustom{ "Conservation mode", ConservationModeButton } },
    "POWER INFORMATION",
    k_FanAnimationFPS,
    []() -> void {},
    [&]() -> ftxui::Canvas { return utils::CustomCanvas(k_CanvasDimentions); });
}
} // namespace ipm::ui::pages
