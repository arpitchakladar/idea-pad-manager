#pragma once

#include <cstddef>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::utils {
struct CanvasSize {
public:
  size_t Width = 0;
  size_t Height = 0;
  static auto fullSize() -> CanvasSize;
  static auto zero() -> CanvasSize;
};

class CustomCanvas : public ftxui::Canvas {
public:
  CustomCanvas(CanvasSize CanvasSize);
  CustomCanvas(const CustomCanvas &) = default;
  CustomCanvas &operator=(const CustomCanvas &) = default;
  CustomCanvas(CustomCanvas &&) = default;
  CustomCanvas &operator=(CustomCanvas &&) = default;
  ~CustomCanvas() = default;

  auto drawFilledTriangle(float X1,
    float Y1,
    float X2,
    float Y2,
    float X3,
    float Y3,
    const ftxui::Color &Color) -> void;
};
} // namespace ipm::ui::utils
