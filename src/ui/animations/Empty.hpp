#pragma once

#include <memory>

#include "ui/animations/CanvasAnimation.hpp"

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
  static auto create() -> std::unique_ptr<Empty> {
    return std::make_unique<Empty>();
  }
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return 0U;
  }
};
} // namespace ipm::ui::animations
