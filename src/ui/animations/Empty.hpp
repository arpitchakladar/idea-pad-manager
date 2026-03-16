#pragma once

#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {
class Empty : public CanvasAnimation {
public:
  Empty() = default;
  Empty(const Empty &) = default;
  Empty &operator=(const Empty &) = default;
  Empty(Empty &&) = default;
  Empty &operator=(Empty &&) = default;
  ~Empty() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return 0U;
  }
};
} // namespace ipm::ui::animations
