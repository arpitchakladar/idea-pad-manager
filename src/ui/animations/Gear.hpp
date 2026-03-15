#pragma once

#include <numbers>

#include "ui/animations/CanvasAnimation.hpp"

namespace ipm::ui::animations {

class Gear : public CanvasAnimation {
public:
  Gear() = default;
  Gear(const Gear &) = default;
  Gear &operator=(const Gear &) = default;
  Gear(Gear &&) = default;
  Gear &operator=(Gear &&) = default;
  ~Gear() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_FPS;
  }

private:
  static constexpr auto k_FPS = 20U;
  static constexpr auto k_PI = std::numbers::pi_v<float>;
  static constexpr auto k_CompleteAngle = 2.0F * k_PI;

  // Helper functions for drawing gear components
  auto drawGearBody(utils::CustomCanvas &Canvas,
    uint CenterX,
    uint CenterY,
    float OuterRadius,
    float InnerRadius) const -> void;
  auto drawGearTeeth(utils::CustomCanvas &Canvas,
    uint CenterX,
    uint CenterY,
    float OuterRadius,
    float ToothHeight,
    uint NumTeeth,
    float ToothWidth) const -> void;
  auto drawCenterCircle(utils::CustomCanvas &Canvas,
    uint CenterX,
    uint CenterY,
    float InnerRadius) const -> void;

  float m_CurrentAngle = 0.0F;
};

} // namespace ipm::ui::animations
