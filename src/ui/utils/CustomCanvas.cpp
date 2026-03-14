#include "ui/utils/CustomCanvas.hpp"

#include <cstddef>
#include <utility>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>

namespace ipm::ui::utils {
auto CanvasSize::fullSize() -> CanvasSize {
  const auto &Screen = ftxui::Terminal::Size();
  return CanvasSize{ .Width = static_cast<size_t>(Screen.dimx * 2),
    .Height = static_cast<size_t>(Screen.dimy * 4) };
}
auto CanvasSize::zero() -> CanvasSize {
  return CanvasSize{ .Width = 0UL, .Height = 0UL };
}

CustomCanvas::CustomCanvas(CanvasSize CanvasSize)
  : ftxui::Canvas(static_cast<int>(CanvasSize.Width),
      static_cast<int>(CanvasSize.Height)) {}

auto CustomCanvas::drawFilledTriangle(float X1,
  float Y1,
  float X2,
  float Y2,
  float X3,
  float Y3,
  const ftxui::Color &Color) -> void {
  if (Y1 > Y2) {
    std::swap(X1, X2);
    std::swap(Y1, Y2);
  }
  if (Y1 > Y3) {
    std::swap(X1, X3);
    std::swap(Y1, Y3);
  }
  if (Y2 > Y3) {
    std::swap(X2, X3);
    std::swap(Y2, Y3);
  }

  if (Y1 == Y3) {
    return;
  }

  for (auto Y = static_cast<int>(Y1), EndY = static_cast<int>(Y3); Y <= EndY;
    ++Y) {
    const auto RealY = static_cast<float>(Y);
    const auto IsBottomHalf = RealY > Y2 || Y2 == Y1;
    const auto SegmentHeight = IsBottomHalf ? (Y3 - Y2) : (Y2 - Y1);

    const auto Alpha = (RealY - Y1) / (Y3 - Y1);
    const auto Beta = (RealY - (IsBottomHalf ? Y2 : Y1)) / SegmentHeight;

    auto Ax = static_cast<int>(X1 + ((X3 - X1) * Alpha));
    auto Bx = static_cast<int>(
      IsBottomHalf ? (X2 + ((X3 - X2) * Beta)) : (X1 + ((X2 - X1) * Beta)));

    if (Ax > Bx) {
      std::swap(Ax, Bx);
    }

    for (auto X = Ax; X <= Bx; ++X) {
      DrawPoint(X, Y, true, Color);
    }
  }
}
} // namespace ipm::ui::utils
