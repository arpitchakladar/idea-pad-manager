#include "ui/pages/PowerInformation.hpp"

#include <cmath>
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

  static constexpr auto k_FanAnimationFPS = 20U;
  static constexpr auto k_PI = std::numbers::pi_v<float>;
  static constexpr auto k_CompleteAngle = 2.0F * k_PI;
  static constexpr auto k_CenterX = 50U;
  static constexpr auto k_CenterY = 50U;
  static constexpr auto k_FanBladeRadius = 36.0F;
  static constexpr auto k_FanCaseRadius = 40.0F;
  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };

  const auto NumBlades = 4U;
  const auto HubRadius = 8.0F;
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

      for (auto I = 0U; I < NumBlades; ++I) {
        const auto BladeAngle = m_CurrentAngle +
          (static_cast<float>(I) * k_CompleteAngle /
            static_cast<float>(NumBlades));

        const auto InnerR = HubRadius;
        const auto OuterR = k_FanBladeRadius;
        const auto WidthAtTip = 0.35F;

        const auto InnerTheta = BladeAngle;
        const auto OuterTheta1 = BladeAngle - WidthAtTip;
        const auto OuterTheta2 = BladeAngle + WidthAtTip;

        const auto X1 =
          static_cast<uint>(k_CenterX + (InnerR * std::cos(InnerTheta)));
        const auto Y1 =
          static_cast<uint>(k_CenterY + (InnerR * std::sin(InnerTheta)));
        const auto X2 =
          static_cast<uint>(k_CenterX + (OuterR * std::cos(OuterTheta1)));
        const auto Y2 =
          static_cast<uint>(k_CenterY + (OuterR * std::sin(OuterTheta1)));
        const auto X3 =
          static_cast<uint>(k_CenterX + (OuterR * std::cos(OuterTheta2)));
        const auto Y3 =
          static_cast<uint>(k_CenterY + (OuterR * std::sin(OuterTheta2)));

        Canvas.drawFilledTriangle(
          X1, Y1, X2, Y2, X3, Y3, ftxui::Color::BlueLight);
      }

      Canvas.DrawPointCircleFilled(k_CenterX,
        k_CenterY,
        static_cast<int>(HubRadius),
        ftxui::Color::DarkBlue);
      Canvas.DrawPointCircle(k_CenterX,
        k_CenterY,
        static_cast<int>(HubRadius) + 1,
        ftxui::Color::Blue);
      Canvas.DrawPointCircle(k_CenterX,
        k_CenterY,
        static_cast<int>(k_FanCaseRadius),
        ftxui::Color::Blue);

      return Canvas;
    });
}
} // namespace ipm::ui::pages
