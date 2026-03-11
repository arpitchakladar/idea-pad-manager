#include <cmath>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/pages/page.hpp"
#include "ui/pages/power_information.hpp"

namespace idea_pad_manager::ui::pages {
PowerInformation::PowerInformation() {
  auto ConservationModeButtonOption = ftxui::ButtonOption::Simple();
  ConservationModeButtonOption.label = &m_ConservationModeButtonMessage;
  ConservationModeButtonOption.on_click = [&] () -> void {
    m_ConservationMode = !m_ConservationMode;
    m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  };
  m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  auto ConservationModeButton =
      ftxui::Button(ConservationModeButtonOption) | ftxui::center;

  m_CurrentAngle = 0.0F;

  createPage(
      {RowDynamic{"Fan Speed", []() -> const char * { return "2100 RPM"; }},
       RowStatic{"Battery Model Name", "L24N4PK3"},
       RowStatic{"Battery Technology", "Li-poly"},
       RowDynamic{"Battery Capacity", []() -> const char * { return "76%"; }},
       RowStatic{"Max battery Capacity", "59Wh"},
       RowStatic{"Manufactured battery Capacity", "60Wh"},
       RowStatic{"Battery charge cycles", "11"},
       RowDynamic{"Battery status",
                  []() -> const char * { return "Not charging"; }},
       RowCustom{"Conservation mode", ConservationModeButton}},
      "POWER INFORMATION", 20,
      [&]() -> void {
        const float Rpm = 2000.0F;
        const float Speed = (Rpm / 1000 / 60.0F) * 2.0F * M_PI;

        m_CurrentAngle += Speed;

        if (m_CurrentAngle > 2.0f * M_PI) {
          m_CurrentAngle -= 2.0F * M_PI;
        }
      },
      [&]() -> ftxui::Canvas {
        const float SegmentsPerBlade = 2.0F;
        const float BladeThickness = 0.5F;
        auto Canvas = ftxui::Canvas(100, 100);
        const int CenterX = 50;
        const int CenterY = 50;
        const int Radius = 40;

        for (int I = 0; I < 3; ++I) {
          float BaseTheta =
              m_CurrentAngle + (I * 2.0F * static_cast<float>(M_PI) / 3.0F);
          for (int J = 0; J < SegmentsPerBlade; ++J) {
            float Offset =
                (static_cast<float>(J) / static_cast<float>(SegmentsPerBlade) -
                 0.5F) *
                BladeThickness;
            float Theta = BaseTheta + Offset;

            int XEnd = CenterX + static_cast<int>(Radius * std::cos(Theta));
            int YEnd = CenterY + static_cast<int>(Radius * std::sin(Theta));

            Canvas.DrawPointLine(CenterX, CenterY, XEnd, YEnd,
                                 ftxui::Color::Cyan);
          }
        }

        return Canvas;
      });
}
} // namespace idea_pad_manager::ui::pages
