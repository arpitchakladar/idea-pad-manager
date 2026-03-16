#pragma once

#include <cstdint>
#include <sys/types.h>

#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

enum class BatteryState : std::uint8_t {
  Charging,
  Discharging,
  NotCharging,
};

class Battery : public CanvasAnimation {
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
    return k_FPS;
  }
  auto setBatteryChargeLevel(std::uint8_t BatteryChargeLevel) -> void {
    m_BatteryChargeLevel = BatteryChargeLevel;
  }
  auto setBatteryState(BatteryState BatteryState) -> void {
    m_BatteryState = BatteryState;
    m_BatteryAnimationChargeLevel = m_BatteryChargeLevel;
  }

private:
  static constexpr auto k_FPS = 10U;
  static constexpr auto k_BatteryWidth = 20U;
  static constexpr auto k_BatteryHeight = 42U;
  static constexpr auto k_BatteryStumpWidth = 10U;
  static constexpr auto k_BatteryStumpHeight = 2U;
  static constexpr auto k_BatteryChargesHeight = 4U;
  static constexpr auto k_BatteryMaxCharge = std::uint8_t(100);
  static constexpr auto k_BatteryMinCharge = std::uint8_t(0);

  std::uint8_t m_BatteryAnimationChargeLevel = k_BatteryMaxCharge;
  std::uint8_t m_BatteryChargeLevel = k_BatteryMaxCharge;

  BatteryState m_BatteryState = BatteryState::Discharging;

  auto drawBattery(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryCharges(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryStump(utils::CustomCanvas &Canvas) const -> void;
};

} // namespace ipm::ui::animations
