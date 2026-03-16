#include "ui/utils/CustomCanvas.hpp"

#include <algorithm>
#include <string>
#include <sys/types.h>
#include <utility>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>
#include <vector>

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

auto CustomCanvas::drawRectangle(
  uint X1, uint Y1, uint X2, uint Y2, const ftxui::Color &Color) -> void {
  const auto MinX = std::min(X1, X2);
  const auto MaxX = std::max(X1, X2);
  const auto MinY = std::min(Y1, Y2);
  const auto MaxY = std::max(Y1, Y2);

  const auto MaxCanvasX = static_cast<uint>(this->width());
  const auto MaxCanvasY = static_cast<uint>(this->height());

  for (auto X = MinX; X <= MaxX && X < MaxCanvasX; ++X) {
    DrawPoint(static_cast<int>(X), static_cast<int>(MinY), true, Color);
    DrawPoint(static_cast<int>(X), static_cast<int>(MaxY), true, Color);
  }
  for (auto Y = MinY; Y <= MaxY && Y < MaxCanvasY; ++Y) {
    DrawPoint(static_cast<int>(MinX), static_cast<int>(Y), true, Color);
    DrawPoint(static_cast<int>(MaxX), static_cast<int>(Y), true, Color);
  }
}

auto CustomCanvas::drawFilledRectangle(
  uint X1, uint Y1, uint X2, uint Y2, const ftxui::Color &Color) -> void {
  const auto MinX = std::min(X1, X2);
  const auto MaxX = std::max(X1, X2);
  const auto MinY = std::min(Y1, Y2);
  const auto MaxY = std::max(Y1, Y2);

  const auto MaxCanvasX = static_cast<uint>(this->width());
  const auto MaxCanvasY = static_cast<uint>(this->height());

  for (auto Y = MinY; Y <= MaxY && Y < MaxCanvasY; ++Y) {
    for (auto X = MinX; X <= MaxX && X < MaxCanvasX; ++X) {
      DrawPoint(static_cast<int>(X), static_cast<int>(Y), true, Color);
    }
  }
}

auto CustomCanvas::drawBigTextCenter(
  uint CenterX, uint CenterY, const std::vector<std::string> &BigText) -> void {
  const auto BigTextWidth =
    static_cast<uint>(std::max(BigText.begin(), BigText.end())->size());
  const auto BigTextHalfWidth = BigTextWidth / 2U;
  static constexpr auto k_VerticalCharacterHeight = 4U;
  const auto BigTextHalfHeight =
    static_cast<uint>(BigText.size() * k_VerticalCharacterHeight) / 2U;
  const auto StartX = CenterX - BigTextHalfWidth;
  const auto StartY = CenterY - BigTextHalfHeight;
  for (uint I = 0U, E = static_cast<uint>(BigText.size()); I < E; ++I) {
    DrawText(static_cast<int>(StartX),
      static_cast<int>(StartY + (I * k_VerticalCharacterHeight)),
      BigText[I]);
  }
}
} // namespace ipm::ui::utils
