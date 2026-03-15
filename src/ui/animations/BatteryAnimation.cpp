#include "ui/animations/BatteryAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <cstdint>
#include <format>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::animations {
auto BatteryAnimation::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto BatteryAnimation::update() -> void {
  static constexpr auto k_BatteryAnimationChargeUnit = std::uint8_t(10);
  switch (m_BatteryState) {
  case BatteryState::Charging:
    if (m_BatteryAnimationChargeLevel >= k_BatteryMaxCharge) {
      m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
    } else {
      m_BatteryAnimationChargeLevel += k_BatteryAnimationChargeUnit;
    }
    break;
  case BatteryState::Discharging:
    if (m_BatteryAnimationChargeLevel <= k_BatteryMinCharge) {
      m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
    } else {
      m_BatteryAnimationChargeLevel -= k_BatteryAnimationChargeUnit;
    }
    break;
  case BatteryState::NotCharging:
    break;
  }
}

auto BatteryAnimation::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  drawBattery(Canvas);
  drawBatteryStump(Canvas);
  drawBatteryCharges(Canvas);
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;

  Canvas.DrawText(static_cast<int>(CenterX) - 4,
    static_cast<int>(CenterY),
    std::format(" {:^3}%", m_BatteryChargeLevel));

  return Canvas;
}

auto BatteryAnimation::drawBattery(utils::CustomCanvas &Canvas) const -> void {
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;
  const auto XOffset = k_BatteryWidth / 2U;
  const auto YOffset = k_BatteryHeight / 2U;

  Canvas.drawRectangle(CenterX - XOffset,
    CenterY - YOffset,
    CenterX + XOffset,
    CenterY + YOffset,
    ftxui::Color::White);
}

auto BatteryAnimation::drawBatteryCharges(utils::CustomCanvas &Canvas) const
  -> void {
  const auto StartingX = ((canvasSize().Width - k_BatteryWidth) / 2U) + 2U;
  const auto EndingX = ((canvasSize().Width + k_BatteryWidth) / 2U) - 2U;
  const auto ChargeHeight =
    ((k_BatteryHeight - 4U) *
      static_cast<uint>(m_BatteryAnimationChargeLevel)) /
    100U;
  const auto BottomY = ((canvasSize().Height + k_BatteryHeight) / 2U) - 2U;
  Canvas.drawFilledRectangle(
    StartingX, BottomY - ChargeHeight, EndingX, BottomY, ftxui::Color::White);
}

auto BatteryAnimation::drawBatteryStump(utils::CustomCanvas &Canvas) const
  -> void {
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY =
    (canvasSize().Height - k_BatteryHeight - k_BatteryStumpHeight) / 2U;
  const auto XOffset = k_BatteryStumpWidth / 2U;
  const auto YOffset = k_BatteryStumpHeight / 2U;

  Canvas.drawFilledRectangle(CenterX - XOffset,
    CenterY - YOffset,
    CenterX + XOffset,
    CenterY + YOffset,
    ftxui::Color::White);
}

} // namespace ipm::ui::animations
