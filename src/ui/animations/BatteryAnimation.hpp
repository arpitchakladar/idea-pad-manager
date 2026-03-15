#pragma once

#include "ui/animations/CanvasAnimation.hpp"
#include <cstdint>

namespace ipm::ui::animations {

class BatteryAnimation : public CanvasAnimation {
public:
  BatteryAnimation() = default;
  BatteryAnimation(const BatteryAnimation &) = default;
  BatteryAnimation &operator=(const BatteryAnimation &) = default;
  BatteryAnimation(BatteryAnimation &&) = default;
  BatteryAnimation &operator=(BatteryAnimation &&) = default;
  ~BatteryAnimation() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_FPS;
  }
  auto setBatteryChargeLevel(std::uint8_t BatteryChargeLevel) -> void {
    m_BatteryChargeLevel = BatteryChargeLevel;
  }

private:
  static constexpr auto k_FPS = 0U;
  static constexpr auto k_BatteryWidth = 20U;
  static constexpr auto k_BatteryHeight = 42U;
  static constexpr auto k_BatteryStumpWidth = 10U;
  static constexpr auto k_BatteryStumpHeight = 2U;
  static constexpr auto k_BatteryChargesHeight = 4U;

  std::uint8_t m_BatteryChargeLevel = std::uint8_t(100);

  auto drawBattery(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryCharges(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryStump(utils::CustomCanvas &Canvas) const -> void;
};

} // namespace ipm::ui::animations
