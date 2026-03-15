#include "ui/animations/GearAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <cmath>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::animations {
namespace {
const ftxui::Color g_GearDarkGray = ftxui::Color::RGB(40, 40, 40);
const ftxui::Color g_GearGray = ftxui::Color::RGB(100, 100, 100);
const ftxui::Color g_GearLightGray = ftxui::Color::RGB(180, 180, 180);
} // namespace

auto GearAnimation::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto GearAnimation::update() -> void {
  const auto Speed = 0.1F;
  m_CurrentAngle += Speed;

  if (m_CurrentAngle > k_CompleteAngle) {
    m_CurrentAngle -= k_CompleteAngle;
  }
}

auto GearAnimation::drawGearBody(utils::CustomCanvas &Canvas,
  uint CenterX,
  uint CenterY,
  float OuterRadius,
  float InnerRadius) const -> void {
  Canvas.DrawPointCircleFilled(static_cast<int>(CenterX),
    static_cast<int>(CenterY),
    static_cast<int>(OuterRadius),
    g_GearDarkGray);
  Canvas.DrawPointCircleFilled(static_cast<int>(CenterX),
    static_cast<int>(CenterY),
    static_cast<int>(InnerRadius),
    ftxui::Color::Black);
  Canvas.DrawPointCircle(static_cast<int>(CenterX),
    static_cast<int>(CenterY),
    static_cast<int>(OuterRadius) + 1,
    g_GearGray);
  Canvas.DrawPointCircle(static_cast<int>(CenterX),
    static_cast<int>(CenterY),
    static_cast<int>(InnerRadius) - 1,
    g_GearGray);
}

auto GearAnimation::drawGearTeeth(utils::CustomCanvas &Canvas,
  uint CenterX,
  uint CenterY,
  float OuterRadius,
  float ToothHeight,
  uint NumTeeth,
  float ToothWidth) const -> void {
  const auto AngleBetweenTeeth = k_CompleteAngle / static_cast<float>(NumTeeth);

  for (auto I = 0U; I < NumTeeth; ++I) {
    const auto ToothAngle = m_CurrentAngle + (I * AngleBetweenTeeth);
    const auto ToothHalfWidth = (AngleBetweenTeeth * ToothWidth) / 2.0F;

    const auto StartAngle = ToothAngle - ToothHalfWidth;
    const auto MidAngle = ToothAngle;
    const auto EndAngle = ToothAngle + ToothHalfWidth;

    // Inner points of tooth
    const auto X1 =
      static_cast<uint>(CenterX + (OuterRadius * std::cos(StartAngle)));
    const auto Y1 =
      static_cast<uint>(CenterY + (OuterRadius * std::sin(StartAngle)));
    const auto X4 =
      static_cast<uint>(CenterX + (OuterRadius * std::cos(EndAngle)));
    const auto Y4 =
      static_cast<uint>(CenterY + (OuterRadius * std::sin(EndAngle)));

    // Outer points of tooth
    const auto X2 = static_cast<uint>(CenterX +
      ((OuterRadius + ToothHeight) * std::cos(MidAngle - ToothHalfWidth / 2)));
    const auto Y2 = static_cast<uint>(CenterY +
      ((OuterRadius + ToothHeight) * std::sin(MidAngle - ToothHalfWidth / 2)));
    const auto X3 = static_cast<uint>(CenterX +
      ((OuterRadius + ToothHeight) * std::cos(MidAngle + ToothHalfWidth / 2)));
    const auto Y3 = static_cast<uint>(CenterY +
      ((OuterRadius + ToothHeight) * std::sin(MidAngle + ToothHalfWidth / 2)));

    // Draw tooth as a filled polygon (approximated with triangles)
    Canvas.drawFilledTriangle(X1, Y1, X2, Y2, CenterX, CenterY, g_GearGray);
    Canvas.drawFilledTriangle(X4, Y4, X3, Y3, CenterX, CenterY, g_GearGray);
    Canvas.drawFilledTriangle(X2, Y2, X3, Y3, CenterX, CenterY, g_GearGray);
  }
}

auto GearAnimation::drawCenterCircle(utils::CustomCanvas &Canvas,
  uint CenterX,
  uint CenterY,
  float InnerRadius) const -> void {
  Canvas.DrawPointCircleFilled(
    CenterX, CenterY, static_cast<int>(InnerRadius * 0.4F), g_GearGray);
  Canvas.DrawPointCircle(
    CenterX, CenterY, static_cast<int>(InnerRadius * 0.4F) + 1, g_GearDarkGray);
}

auto GearAnimation::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  // Calculate center based on actual canvas size
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;

  // Increased gear size parameters (scaled to canvas size)
  static constexpr auto k_OuterRadiusRatio =
    0.65F; // Increased from 0.35F to 0.45F
  static constexpr auto k_InnerRadiusRatio =
    0.45F; // Increased from 0.20F to 0.25F
  static constexpr auto k_NumTeeth = 8U;
  static constexpr auto k_ToothWidth =
    0.25F; // Proportion of tooth width to angle between teeth
  static constexpr auto k_ToothHeightRatio =
    0.12F; // Increased from 0.08F to 0.12F

  const auto k_OuterRadius = std::min(canvasSize().Width, canvasSize().Height) *
    k_OuterRadiusRatio / 2.0F;
  const auto k_InnerRadius = std::min(canvasSize().Width, canvasSize().Height) *
    k_InnerRadiusRatio / 2.0F;
  const auto k_ToothHeight = std::min(canvasSize().Width, canvasSize().Height) *
    k_ToothHeightRatio / 2.0F;

  // Draw gear components using helper functions
  drawGearTeeth(Canvas,
    CenterX,
    CenterY,
    k_OuterRadius,
    k_ToothHeight,
    k_NumTeeth,
    k_ToothWidth);
  drawGearBody(Canvas, CenterX, CenterY, k_OuterRadius, k_InnerRadius);
  drawCenterCircle(Canvas, CenterX, CenterY, k_InnerRadius);

  return Canvas;
}
} // namespace ipm::ui::animations
