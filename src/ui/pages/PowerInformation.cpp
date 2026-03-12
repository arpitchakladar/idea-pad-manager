#include "ui/pages/PowerInformation.hpp"

#include <cmath>
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
  m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  auto ConservationModeButton =
    ftxui::Button(ConservationModeButtonOption) | ftxui::center;

  m_CurrentAngle = 0.0F;

  createPage(
    { RowDynamic{ "Fan Speed", []() -> const char * { return "2100 RPM"; } },
      RowStatic{ "Battery Model Name", "L24N4PK3" },
      RowStatic{ "Battery Technology", "Li-poly" },
      RowDynamic{ "Battery Capacity", []() -> const char * { return "76%"; } },
      RowStatic{ "Max battery Capacity", "59Wh" },
      RowStatic{ "Manufactured battery Capacity", "60Wh" },
      RowStatic{ "Battery charge cycles", "11" },
      RowDynamic{
        "Battery status", []() -> const char * { return "Not charging"; } },
      RowCustom{ "Conservation mode", ConservationModeButton } },
    "POWER INFORMATION",
    20,
    [&]() -> void {
      constexpr auto Rpm = 2000.0F;
      constexpr auto Speed =
        (Rpm / 1000.0F / 60.0F) * 2.0F * std::numbers::pi_v<float>;

      m_CurrentAngle += Speed;

      if (m_CurrentAngle > 2.0f * M_PI) {
        m_CurrentAngle -= 2.0F * M_PI;
      }
    },
    [&]() -> ftxui::Canvas {
      constexpr auto PI = std::numbers::pi_v<float>;
      constexpr auto BladeThickness = PI / 24.0F;
      auto Canvas = utils::CustomCanvas(100, 100);
      constexpr auto CenterX = 50;
      constexpr auto CenterY = 50;
      constexpr auto Radius = 40.0F;

      for (auto I = 0; I < 3; ++I) {
        const auto BaseTheta =
          m_CurrentAngle + (static_cast<float>(I) * 2.0F * PI / 3.0F);
        const auto Theta = std::make_pair(
          BaseTheta - BladeThickness, BaseTheta + BladeThickness);
        Canvas.DrawFilledTriangle(CenterX,
          CenterY,
          CenterX + Radius * std::cos(Theta.first),
          CenterY + Radius * std::sin(Theta.first),
          CenterX + Radius * std::cos(Theta.second),
          CenterY + Radius * std::sin(Theta.second),
          ftxui::Color::Blue);
      }

      return Canvas;
    });
}
} // namespace ipm::ui::pages
