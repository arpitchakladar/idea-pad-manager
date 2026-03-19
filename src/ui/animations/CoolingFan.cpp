#include "ui/animations/CoolingFan.hpp"

#include <cmath>

#include <ftxui/screen/color.hpp>
#include <sys/types.h>

#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {
auto CoolingFan::resize(utils::CanvasSize CanvasSize) -> void {
  Animation::resize(CanvasSize);
}

auto CoolingFan::update() -> void {
  const auto Speed = (m_Rpm / 1000.0F / 60.0F) * 2.0F * k_PI;
  m_CurrentAngle += Speed;

  if (m_CurrentAngle > k_CompleteAngle) {
    m_CurrentAngle -= k_CompleteAngle;
  }
}

auto CoolingFan::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  for (auto I = 0U; I < k_NumBlades; ++I) {
    const auto BladeAngle = m_CurrentAngle +
      (static_cast<float>(I) * k_CompleteAngle /
        static_cast<float>(k_NumBlades));

    const auto InnerR = k_HubRadius;
    const auto OuterR = k_FanBladeRadius;

    const auto InnerTheta = BladeAngle;
    const auto OuterTheta1 = BladeAngle - k_WidthAtTip;
    const auto OuterTheta2 = BladeAngle + k_WidthAtTip;

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

    Canvas.drawFilledTriangle(X1, Y1, X2, Y2, X3, Y3, ftxui::Color::BlueLight);
  }

  Canvas.DrawPointCircleFilled(k_CenterX,
    k_CenterY,
    static_cast<int>(k_HubRadius),
    ftxui::Color::DarkBlue);
  Canvas.DrawPointCircle(k_CenterX,
    k_CenterY,
    static_cast<int>(k_HubRadius + 1U),
    ftxui::Color::Blue);
  Canvas.DrawPointCircle(k_CenterX,
    k_CenterY,
    static_cast<int>(k_FanCaseRadius),
    ftxui::Color::Blue);

  return Canvas;
}

} // namespace ipm::ui::animations
