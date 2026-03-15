#include "ui/pages/ThermalPerformance.hpp"

#include <cmath>

#include <ftxui/dom/canvas.hpp>

#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
ThermalPerformance::ThermalPerformance() {
  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  static constexpr auto k_FanAnimationFPS = 20U;
  static constexpr auto k_PI = std::numbers::pi_v<float>;
  static constexpr auto k_CompleteAngle = 2.0F * k_PI;
  static constexpr auto k_CenterX = 50U;
  static constexpr auto k_CenterY = 50U;
  static constexpr auto k_FanBladeRadius = 36.0F;
  static constexpr auto k_FanCaseRadius = 40.0F;

  const auto NumBlades = 4U;
  const auto HubRadius = 8.0F;
  const auto Rpm = 2000.0F;
  const auto Speed = (Rpm / 1000.0F / 60.0F) * 2.0F * k_PI;
  createPage(
    { RowDynamic{ "CPU Temperature", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 1", []() -> const char * { return "64°C"; } },
      RowDynamic{ "CPU Core 2", []() -> const char * { return "66°C"; } },
      RowDynamic{ "CPU Core 3", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 4", []() -> const char * { return "67°C"; } },
      RowDynamic{ "CPU Core 5", []() -> const char * { return "64°C"; } },
      RowDynamic{ "CPU Core 6", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 7", []() -> const char * { return "66°C"; } },
      RowDynamic{ "CPU Core 8", []() -> const char * { return "68°C"; } },
      RowDynamic{ "GPU Temperature", []() -> const char * { return "58°C"; } },
      RowStatic{ "Thermal Mode", "Balanced" },
      RowStatic{ "CPU Frequency", "4.2 GHz" },
      RowStatic{ "CPU Core 1", "4.1 GHz" },
      RowStatic{ "CPU Core 2", "4.3 GHz" },
      RowStatic{ "CPU Core 3", "4.2 GHz" },
      RowStatic{ "CPU Core 4", "4.1 GHz" },
      RowStatic{ "CPU Core 5", "4.2 GHz" },
      RowStatic{ "CPU Core 6", "4.3 GHz" },
      RowStatic{ "CPU Core 7", "4.2 GHz" },
      RowStatic{ "CPU Core 8", "4.1 GHz" },
      RowStatic{ "CPU Governor", "performance" },
      RowStatic{ "Min Frequency", "800 MHz" },
      RowStatic{ "Max Frequency", "5.0 GHz" } },
    "Thermal & Performance",
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
