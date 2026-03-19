#include "ui/animations/Battery.hpp"

#include <algorithm>
#include <cstdint>
#include <format>

#include <ftxui/screen/color.hpp>
#include <sys/types.h>

#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {
auto Battery::resize(utils::CanvasSize CanvasSize) -> void {
  Animation::resize(CanvasSize);
  m_BatteryWidth = std::min(k_DefaultBatteryWidth, CanvasSize.Width);
  m_BatteryHeight = std::min(k_DefaultBatteryHeight, CanvasSize.Height);
  m_BatteryStumpWidth = m_BatteryWidth / k_DefaultBatteryStumpWidthDivisor;
  m_BatteryStumpHeight = k_DefaultBatteryStumpHeight;
}

auto Battery::update() -> void {
  static constexpr auto k_BatteryChargeUnit = std::uint8_t(10);
  switch (m_BatteryState) {
  case BatteryState::Charging:
    if (m_BatteryAnimationChargeLevel >= k_BatteryMaxCharge) {
      m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
    } else {
      m_BatteryAnimationChargeLevel += k_BatteryChargeUnit;
    }
    break;
  case BatteryState::Discharging:
    if (m_BatteryAnimationChargeLevel <= k_BatteryMinCharge) {
      m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
    } else {
      m_BatteryAnimationChargeLevel -= k_BatteryChargeUnit;
    }
    break;
  case BatteryState::NotCharging:
    break;
  }
}

auto Battery::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  drawBattery(Canvas);
  drawBatteryStump(Canvas);
  drawBatteryCharges(Canvas);
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;

  Canvas.DrawText(static_cast<int>(CenterX) - 4,
    static_cast<int>(CenterY),
    std::format("{:^3} %", m_BatteryChargeLevel));

  return Canvas;
}

auto Battery::drawBattery(utils::CustomCanvas &Canvas) const -> void {
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;
  const auto XOffset = m_BatteryWidth / 2U;
  const auto YOffset = m_BatteryHeight / 2U;

  Canvas.drawRectangle(CenterX - XOffset,
    CenterY - YOffset,
    CenterX + XOffset,
    CenterY + YOffset,
    ftxui::Color::White);
}

auto Battery::drawBatteryCharges(utils::CustomCanvas &Canvas) const -> void {
  const auto StartingX = ((canvasSize().Width - m_BatteryWidth) / 2U) + 2U;
  const auto EndingX = ((canvasSize().Width + m_BatteryWidth) / 2U) - 2U;
  const auto ChargeHeight =
    ((m_BatteryHeight - 4U) *
      static_cast<uint>(m_BatteryAnimationChargeLevel)) /
    100U;
  const auto BottomY = ((canvasSize().Height + m_BatteryHeight) / 2U) - 2U;
  Canvas.drawFilledRectangle(
    StartingX, BottomY - ChargeHeight, EndingX, BottomY, ftxui::Color::White);
}

auto Battery::drawBatteryStump(utils::CustomCanvas &Canvas) const -> void {
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY =
    (canvasSize().Height - m_BatteryHeight - m_BatteryStumpHeight) / 2U;
  const auto XOffset = m_BatteryStumpWidth / 2U;
  const auto YOffset = m_BatteryStumpHeight / 2U;

  Canvas.drawFilledRectangle(CenterX - XOffset,
    CenterY - YOffset,
    CenterX + XOffset,
    CenterY + YOffset,
    ftxui::Color::White);
}

} // namespace ipm::ui::animations
