#pragma once

#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class CanvasAnimation {
public:
  virtual ~CanvasAnimation() = default;

  virtual auto update() -> void = 0;
  [[nodiscard]] virtual auto drawCanvas() const -> utils::CustomCanvas = 0;
  virtual auto resize(utils::CanvasSize CanvasSize) -> void {
    m_CanvasSize = CanvasSize;
  }
  [[nodiscard]] virtual auto canvasUpdatesPerSecond() const -> int = 0;

protected:
  CanvasAnimation() = default;

  [[nodiscard]] auto canvasSize() const -> utils::CanvasSize {
    return m_CanvasSize;
  }

private:
  utils::CanvasSize m_CanvasSize;
};

} // namespace ipm::ui::animations
