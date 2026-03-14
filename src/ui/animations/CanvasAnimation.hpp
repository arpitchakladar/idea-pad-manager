#pragma once

#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class CanvasAnimation {
public:
  CanvasAnimation() = default;
  CanvasAnimation(const CanvasAnimation &) = default;
  CanvasAnimation &operator=(const CanvasAnimation &) = default;
  CanvasAnimation(CanvasAnimation &&) = default;
  CanvasAnimation &operator=(CanvasAnimation &&) = default;
  virtual ~CanvasAnimation() = default;

  virtual auto update() -> void = 0;
  [[nodiscard]] virtual auto drawCanvas() const -> utils::CustomCanvas = 0;
  virtual auto resize(utils::CanvasSize CanvasSize) -> void {
    m_CanvasSize = CanvasSize;
  }
  [[nodiscard]] virtual auto canvasUpdatesPerSecond() const -> int = 0;

protected:
  [[nodiscard]] auto canvasSize() const -> utils::CanvasSize {
    return m_CanvasSize;
  }

private:
  utils::CanvasSize m_CanvasSize = utils::CanvasSize::zero();
};

} // namespace ipm::ui::animations
