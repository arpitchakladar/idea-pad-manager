#pragma once

#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"
#include <sys/types.h>

namespace ipm::ui::animations {
class Empty : public Animation {
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
