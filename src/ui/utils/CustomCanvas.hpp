#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::utils {
struct CanvasSize {
public:
  size_t Width;
  size_t Height;
  static auto fullSize() -> CanvasSize;
};

class CustomCanvas : public ftxui::Canvas {
public:
  CustomCanvas(CanvasSize CanvasSize);
  auto drawFilledTriangle(float X1,
    float Y1,
    float X2,
    float Y2,
    float X3,
    float Y3,
    const ftxui::Color &Color) -> void;
};
} // namespace ipm::ui::utils
