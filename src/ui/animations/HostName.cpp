#include "ui/animations/HostName.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::animations {

namespace {
const std::vector<std::string> g_LinuxSmall = { " _    _                     ",
  "| |  (_)_ __  _   ___  __  ",
  "| |__| | '  \\| | | \\ \\/ /  ",
  "|____|_|_|_|_|\\_,_|/_/\\_\\  " };
}

auto HostName::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
}

auto HostName::update() -> void {}

auto HostName::drawCanvas() const -> utils::CustomCanvas {
  auto Canvas = utils::CustomCanvas(canvasSize());
  const auto CenterX = canvasSize().Width / 2U;
  const auto CenterY = canvasSize().Height / 2U;

  Canvas.drawBigTextCenter(CenterX, CenterY, g_LinuxSmall);

  return Canvas;
}
} // namespace ipm::ui::animations
