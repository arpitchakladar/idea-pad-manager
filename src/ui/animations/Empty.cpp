#include "ui/animations/Empty.hpp"

#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {
auto Empty::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto Empty::update() -> void {}

auto Empty::drawCanvas() const -> utils::CustomCanvas {
  return utils::CustomCanvas(canvasSize());
}
} // namespace ipm::ui::animations
