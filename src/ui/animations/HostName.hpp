#pragma once

#include <numbers>

#include "ui/animations/CanvasAnimation.hpp"

namespace ipm::ui::animations {

class HostName : public CanvasAnimation {
public:
  HostName() = default;
  HostName(const HostName &) = default;
  HostName &operator=(const HostName &) = default;
  HostName(HostName &&) = default;
  HostName &operator=(HostName &&) = default;
  ~HostName() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_FPS;
  }

private:
  static constexpr auto k_FPS = 0U;
};

} // namespace ipm::ui::animations
