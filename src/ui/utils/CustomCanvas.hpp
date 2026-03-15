#pragma once

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::utils {
struct CanvasSize {
public:
  uint Width = 0;
  uint Height = 0;
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

  auto drawFilledTriangle(uint X1,
    uint Y1,
    uint X2,
    uint Y2,
    uint X3,
    uint Y3,
    const ftxui::Color &Color) -> void;
};
} // namespace ipm::ui::utils
