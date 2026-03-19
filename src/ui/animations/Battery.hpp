#pragma once

#include <cstdint>
#include <sys/types.h>

#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

enum class BatteryState : std::uint8_t {
  Charging,
  Discharging,
  NotCharging,
};

class Battery : public Animation {
public:
  Battery() = default;
  Battery(const Battery &) = default;
  Battery &operator=(const Battery &) = default;
  Battery(Battery &&) = default;
  Battery &operator=(Battery &&) = default;
  ~Battery() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_CanvasUpdatesPerSecond;
  }
  auto setBatteryChargeLevel(std::uint8_t BatteryChargeLevel) -> void {
    m_BatteryChargeLevel = BatteryChargeLevel;
  }
  auto setBatteryState(BatteryState BatteryState) -> void {
    m_BatteryState = BatteryState;
    m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
  }

private:
  static constexpr auto k_CanvasUpdatesPerSecond = 10U;
  static constexpr auto k_DefaultBatteryWidth = 20U;
  static constexpr auto k_DefaultBatteryHeight = 42U;
  static constexpr auto k_DefaultBatteryStumpWidthDivisor = 2U;
  static constexpr auto k_DefaultBatteryStumpHeight = 2U;
  static constexpr auto k_BatteryMaxCharge = std::uint8_t(100);
  static constexpr auto k_BatteryMinCharge = std::uint8_t(0);

  uint m_BatteryWidth = k_DefaultBatteryWidth;
  uint m_BatteryHeight = k_DefaultBatteryHeight;
  uint m_BatteryStumpWidth =
    k_DefaultBatteryWidth / k_DefaultBatteryStumpWidthDivisor;
  uint m_BatteryStumpHeight = k_DefaultBatteryStumpHeight;

  std::uint8_t m_BatteryAnimationChargeLevel = k_BatteryMaxCharge;
  std::uint8_t m_BatteryChargeLevel = k_BatteryMaxCharge;

  BatteryState m_BatteryState = BatteryState::Discharging;

  auto drawBattery(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryCharges(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryStump(utils::CustomCanvas &Canvas) const -> void;
};

} // namespace ipm::ui::animations
