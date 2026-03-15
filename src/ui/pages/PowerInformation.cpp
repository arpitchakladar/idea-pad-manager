#include "ui/pages/PowerInformation.hpp"

#include <cmath>
#include <numbers>
#include <utility>

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

  static constexpr auto k_FanAnimationFPS = 20U;
  static constexpr auto k_PI = std::numbers::pi_v<float>;
  static constexpr auto k_CompleteAngle = 2.0F * k_PI;
  static constexpr auto k_BladeThickness = k_PI / 24.0F;
  static constexpr auto k_CenterX = 50U;
  static constexpr auto k_CenterY = 50U;
  static constexpr auto k_Radius = 40.0F;
  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };

  const auto Rpm = 2000.0F;
  const auto Speed = (Rpm / 1000.0F / 60.0F) * 2.0F * k_PI;

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
    [&]() -> void {
      m_CurrentAngle += Speed;

      if (m_CurrentAngle > k_CompleteAngle) {
        m_CurrentAngle -= k_CompleteAngle;
      }
    },
    [&]() -> ftxui::Canvas {
      auto Canvas = utils::CustomCanvas(k_CanvasDimentions);
      for (auto I = 0; I < 3; ++I) {
        const auto BaseTheta =
          m_CurrentAngle + (static_cast<float>(I) * 2.0F * k_PI / 3.0F);
        const auto Theta = std::make_pair(
          BaseTheta - k_BladeThickness, BaseTheta + k_BladeThickness);
        Canvas.drawFilledTriangle(k_CenterX,
          k_CenterY,
          k_CenterX + (k_Radius * std::cos(Theta.first)),
          k_CenterY + (k_Radius * std::sin(Theta.first)),
          k_CenterX + (k_Radius * std::cos(Theta.second)),
          k_CenterY + (k_Radius * std::sin(Theta.second)),
          ftxui::Color::Blue);
      }

      return Canvas;
    });
}
} // namespace ipm::ui::pages
