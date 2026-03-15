#include "ui/utils/CustomCanvas.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>

namespace ipm::ui::utils {
auto CanvasSize::fullSize() -> CanvasSize {
  const auto &Screen = ftxui::Terminal::Size();
  return CanvasSize{ .Width = static_cast<uint>(Screen.dimx * 2),
    .Height = static_cast<uint>(Screen.dimy * 4) };
}
auto CanvasSize::zero() -> CanvasSize {
  return CanvasSize{ .Width = 0U, .Height = 0U };
}

CustomCanvas::CustomCanvas(CanvasSize CanvasSize)
  : ftxui::Canvas(static_cast<int>(CanvasSize.Width),
      static_cast<int>(CanvasSize.Height)) {}

auto CustomCanvas::drawFilledTriangle(uint X1,
  uint Y1,
  uint X2,
  uint Y2,
  uint X3,
  uint Y3,
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

  const auto Fx1 = static_cast<float>(X1);
  const auto Fy1 = static_cast<float>(Y1);
  const auto Fx2 = static_cast<float>(X2);
  const auto Fy2 = static_cast<float>(Y2);
  const auto Fx3 = static_cast<float>(X3);
  const auto Fy3 = static_cast<float>(Y3);

  const auto TotalHeight = Fy3 - Fy1;
  if (TotalHeight == 0.0F) {
    return;
  }

  const auto MaxX = static_cast<uint>(this->width());
  const auto MaxY = static_cast<uint>(this->height());

  for (auto Y = Y1; Y <= Y3 && Y < MaxY; ++Y) {
    const auto Fy = static_cast<float>(Y);

    const auto Alpha = (Fy - Fy1) / TotalHeight;
    auto Ax = Fx1 + ((Fx3 - Fx1) * Alpha);

    auto Bx = 0.0F;
    if (Y <= Y2) {
      const auto SegH = Fy2 - Fy1;
      Bx = (SegH > 0.0F) ? (Fx1 + ((Fx2 - Fx1) * ((Fy - Fy1) / SegH))) : Fx2;
    } else {
      const auto SegH = Fy3 - Fy2;
      Bx = (SegH > 0.0F) ? (Fx2 + ((Fx3 - Fx2) * ((Fy - Fy2) / SegH))) : Fx3;
    }

    if (Ax > Bx) {
      std::swap(Ax, Bx);
    }

    for (auto X = static_cast<uint>(Ax); X <= static_cast<uint>(Bx) && X < MaxX;
      ++X) {
      DrawPoint(static_cast<int>(X), static_cast<int>(Y), true, Color);
    }
  }
}
} // namespace ipm::ui::utils
