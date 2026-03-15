#include "ui/animations/BatteryAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <cmath>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::animations {
auto BatteryAnimation::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto BatteryAnimation::update() -> void {}

auto BatteryAnimation::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());

  drawBattery(Canvas);

  return Canvas;
}

auto BatteryAnimation::drawBattery(utils::CustomCanvas &Canvas) const -> void {
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;
  const auto XOffset = k_BatteryWidth / 2U;
  const auto YOffset = k_BatteryHeight / 2U;

  Canvas.drawRectangle(CenterX - XOffset,
    CenterY - YOffset,
    CenterX + XOffset,
    CenterY + YOffset,
    ftxui::Color::White);
}

} // namespace ipm::ui::animations
