#pragma once

#include "ui/utils/CustomCanvas.hpp"
#include <sys/types.h>

namespace ipm::ui::animations {

class Animation {
public:
  Animation() = default;
  Animation(const Animation &) = default;
  Animation &operator=(const Animation &) = default;
  Animation(Animation &&) = default;
  Animation &operator=(Animation &&) = default;
  virtual ~Animation() = default;

  virtual auto update() -> void = 0;
  [[nodiscard]] virtual auto drawCanvas() const -> utils::CustomCanvas = 0;
  virtual auto resize(utils::CanvasSize CanvasSize) -> void {
    m_CanvasSize = CanvasSize;
  }
  [[nodiscard]] virtual auto canvasUpdatesPerSecond() const -> uint = 0;

protected:
  [[nodiscard]] auto canvasSize() const -> utils::CanvasSize {
    return m_CanvasSize;
  }

private:
  utils::CanvasSize m_CanvasSize = utils::CanvasSize::zero();
};

} // namespace ipm::ui::animations
