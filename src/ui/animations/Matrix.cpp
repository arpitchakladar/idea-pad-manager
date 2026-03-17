#include "ui/animations/Matrix.hpp"
#include "ui/animations/Animation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <chrono>
#include <cstddef>
#include <ftxui/screen/color.hpp>
#include <sys/types.h>

namespace ipm::ui::animations {

namespace {
const ftxui::Color g_GreenDim = ftxui::Color::RGB(0, 100, 0);
const ftxui::Color g_GreenBright = ftxui::Color::RGB(0, 255, 0);
} // namespace

Matrix::Matrix() { m_LastTime = std::chrono::steady_clock::now(); }

auto Matrix::resize(utils::CanvasSize CanvasSize) -> void {
  Animation::resize(CanvasSize);

  m_Columns.clear();
  m_Columns.reserve(CanvasSize.Width);
  m_CharBuffer.assign(static_cast<std::size_t>(CanvasSize.Width) *
      static_cast<std::size_t>(CanvasSize.Height),
    0U);

  for (auto X = 0U; X < CanvasSize.Width; ++X) {
    auto Col = Column{
      .X = X,
      .Y = 0,
      .Speed = static_cast<uint>(m_SpeedDist(m_Rng)),
      .Length = static_cast<uint>(m_LengthDist(m_Rng)),
    };
    m_Columns.push_back(Col);
  }
}

auto Matrix::update() -> void {
  static constexpr auto k_Delta = 1.0F / 30.0F;
  const auto Now = std::chrono::steady_clock::now();
  const auto ElapsedTime =
    std::chrono::duration<float>(Now - m_LastTime).count();
  if (ElapsedTime < k_Delta) {
    return;
  }

  const auto CanvasSize = canvasSize();
  m_FrameCount++;

  for (auto &Col : m_Columns) {
    Col.Y += Col.Speed;

    if (Col.Y >= CanvasSize.Height) {
      Col.Y = 0U;
      Col.Speed = m_SpeedDist(m_Rng);
      Col.Length = m_LengthDist(m_Rng);
    }

    const auto StartY = Col.Y > Col.Length ? Col.Y - Col.Length : 0U;
    for (auto Y = StartY; Y <= Col.Y && Y < CanvasSize.Height; ++Y) {
      if (Y == Col.Y || (m_FrameCount % k_CharRefreshRate == 0U)) {
        const auto Idx = (Y * CanvasSize.Width) + Col.X;
        m_CharBuffer[Idx] = m_CharDist(m_Rng);
      }
    }

    if (Col.Y >= CanvasSize.Height) {
      const auto Idx = ((CanvasSize.Height - 1U) * CanvasSize.Width) + Col.X;
      m_CharBuffer[Idx] = 0;
    }
  }

  m_LastTime = Now;
}

auto Matrix::drawCanvas() const -> utils::CustomCanvas {
  const auto CanvasSize = canvasSize();
  auto Canvas = utils::CustomCanvas(CanvasSize);

  for (const auto &Col : m_Columns) {
    const auto StartY = Col.Y > Col.Length ? Col.Y - Col.Length : 0U;
    for (auto Y = StartY; Y <= Col.Y && Y < CanvasSize.Height; ++Y) {
      const auto Idx = (Y * CanvasSize.Width) + Col.X;
      const auto Ch = static_cast<char>(m_CharBuffer[Idx]);
      if (Ch == 0) {
        continue;
      }

      const auto IsHead = (Y == Col.Y);
      const auto IsNearHead = (Col.Y - Y < 3);
      const auto &Color = (IsHead || IsNearHead) ? g_GreenBright : g_GreenDim;

      Canvas.DrawPoint(
        static_cast<int>(Col.X), static_cast<int>(Y), true, Color);
    }
  }

  return Canvas;
}

auto Matrix::initColumn(Column &Col) -> void {
  Col.Y = 0;
  Col.Speed = m_SpeedDist(m_Rng);
  Col.Length = m_LengthDist(m_Rng);
}

} // namespace ipm::ui::animations
