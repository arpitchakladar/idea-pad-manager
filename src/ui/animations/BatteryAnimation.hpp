#pragma once

#include "ui/animations/CanvasAnimation.hpp"

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

private:
  static constexpr auto k_FPS = 20U;
  static constexpr auto k_BatteryWidth = 20U;
  static constexpr auto k_BatteryHeight = 40U;
  static constexpr auto k_BatteryStumpWidth = 10U;
  static constexpr auto k_BatteryStumpHeight = 2U;

  auto drawBattery(utils::CustomCanvas &Canvas) const -> void;
  auto drawBatteryStump(utils::CustomCanvas &Canvas) const -> void;
};

} // namespace ipm::ui::animations
