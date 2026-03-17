#pragma once

#include <numbers>
#include <sys/types.h>

#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class CoolingFan : public Animation {
public:
  CoolingFan() = default;
  CoolingFan(const CoolingFan &) = default;
  CoolingFan &operator=(const CoolingFan &) = default;
  CoolingFan(CoolingFan &&) = default;
  CoolingFan &operator=(CoolingFan &&) = default;
  ~CoolingFan() override = default;

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
  static constexpr auto k_CenterX = 50U;
  static constexpr auto k_CenterY = 50U;
  static constexpr auto k_FanBladeRadius = 36.0F;
  static constexpr auto k_FanCaseRadius = 40.0F;
  static constexpr auto k_NumBlades = 4U;
  static constexpr auto k_HubRadius = 8.0F;
  static constexpr auto k_WidthAtTip = 0.35F;
  static constexpr auto k_DefaultRPM = 2000.0F;

  float m_CurrentAngle = 0.0F;
  float m_Rpm = k_DefaultRPM;
};

} // namespace ipm::ui::animations
