#include "ui/utils/CustomCanvas.hpp"

#include <utility>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

namespace ipm::ui::utils {
CustomCanvas::CustomCanvas(int Width, int Height)
  : ftxui::Canvas(Width, Height) {}

auto CustomCanvas::DrawFilledTriangle(float X1,
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
    const auto IsBottomHalf = static_cast<float>(Y) > Y2 || Y2 == Y1;
    const auto SegmentHeight = IsBottomHalf ? (Y3 - Y2) : (Y2 - Y1);

    const auto Alpha = (static_cast<float>(Y) - Y1) / (Y3 - Y1);
    const auto Beta =
      (static_cast<float>(Y) - (IsBottomHalf ? Y2 : Y1)) / SegmentHeight;

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
