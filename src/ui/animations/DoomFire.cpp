#include "ui/animations/DoomFire.hpp"
#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <utility>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/pixel.hpp>

namespace ipm::ui::animations {

DoomFire::DoomFire() { m_LastTime = std::chrono::steady_clock::now(); }

auto DoomFire::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
  m_Buffer.assign(static_cast<std::size_t>(CanvasSize.Width) *
      static_cast<std::size_t>(CanvasSize.Height),
    std::uint8_t(0));
  buildPalette();
  seedBottomRow();
}

auto DoomFire::update() -> void {
  if (m_Buffer.empty()) {
    return;
  }
  static constexpr auto k_Delta = 1.0F / 20.0F;
  const auto Now = std::chrono::steady_clock::now();
  const auto ElapsedTime =
    std::chrono::duration<float>(Now - m_LastTime).count();
  if (ElapsedTime < k_Delta) {
    return;
  }
  const auto CanvasSize = canvasSize();
  for (auto X = 0U; X < CanvasSize.Width; ++X) {
    for (auto Y = 1U; Y < CanvasSize.Height; ++Y) {
      spreadFire((Y * CanvasSize.Width) + X);
    }
  }
  m_LastTime = Now;
}

auto DoomFire::drawCanvas() const -> utils::CustomCanvas {
  const auto CanvasSize = canvasSize();
  auto Canvas = utils::CustomCanvas(CanvasSize);
  if (m_Buffer.empty()) {
    return Canvas;
  }
  for (auto X = 0U; X < CanvasSize.Width; ++X) {
    for (auto Y = 0U; Y < CanvasSize.Height; ++Y) {
      const auto Intensity = m_Buffer[(Y * CanvasSize.Width) + X];
      if (Intensity == 0) {
        continue;
      }

      const ftxui::Color C = m_Palette[Intensity];

      Canvas.DrawBlock(static_cast<int>(X),
        static_cast<int>(Y),
        true,
        [C](ftxui::Pixel &P) -> void {
          P.background_color = C;
          P.foreground_color = C;
        });
    }
  }

  return Canvas;
}

auto DoomFire::seedBottomRow() -> void {
  const auto CanvasSize = canvasSize();
  for (auto X = 0U; X < CanvasSize.Width; ++X) {
    m_Buffer[((CanvasSize.Height - 1U) * CanvasSize.Width) + X] =
      k_MaxIntensity;
  }
}

auto DoomFire::spreadFire(uint SrcIdx) -> void {
  const auto CanvasSize = canvasSize();
  const auto SrcIntensity = m_Buffer[SrcIdx];

  if (SrcIntensity == 0) {
    m_Buffer[SrcIdx - CanvasSize.Width] = 0;
    return;
  }

  const auto Decay = m_DecayDist(m_Rng);
  const auto Wind = m_WindDist(m_Rng);

  const auto DstX = static_cast<int>(SrcIdx % CanvasSize.Width) + Wind;
  const auto DstY = (SrcIdx / CanvasSize.Width) - 1U;

  if (DstX < 0 || DstX >= static_cast<int>(CanvasSize.Width)) {
    return;
  }

  const auto DstIdx = (DstY * CanvasSize.Width) + DstX;
  const auto NewIntensity = static_cast<std::uint8_t>(
    std::max(0, static_cast<int>(SrcIntensity) - static_cast<int>(Decay)));

  m_Buffer[DstIdx] = NewIntensity;
}

auto DoomFire::buildPalette() -> void {
  m_Palette.resize(static_cast<uint>(k_MaxIntensity) + 1U);
  m_Palette[0] = ftxui::Color::Black;

  static constexpr auto k_Q1Threshold = 0.25F;
  static constexpr auto k_Q2Threshold = 0.50F;
  static constexpr auto k_Q3Threshold = 0.75F;

  static constexpr auto k_Q1MaxRed = 160.0F;
  static constexpr auto k_Q2RedBase = 160.0F;
  static constexpr auto k_Q2RedRange = 95.0F;
  static constexpr auto k_Q2GreenRange = 60.0F;
  static constexpr auto k_Q3GreenBase = 60.0F;
  static constexpr auto k_Q3GreenRange = 140.0F;
  static constexpr auto k_Q4GreenBase = 200.0F;
  static constexpr auto k_Q4GreenRange = 55.0F;
  static constexpr auto k_Q4BlueRange = 180.0F;
  static constexpr auto k_FullChannel = 255U;

  for (auto I = 1U; std::cmp_less_equal(I, k_MaxIntensity); ++I) {
    const auto T = static_cast<float>(I) / static_cast<float>(k_MaxIntensity);
    auto R = std::uint8_t(0);
    auto G = std::uint8_t(0);
    auto B = std::uint8_t(0);

    if (T < k_Q1Threshold) {
      R = static_cast<std::uint8_t>(T / k_Q1Threshold * k_Q1MaxRed);
    } else if (T < k_Q2Threshold) {
      const auto S = (T - k_Q1Threshold) / k_Q1Threshold;
      R = static_cast<std::uint8_t>(k_Q2RedBase + (S * k_Q2RedRange));
      G = static_cast<std::uint8_t>(S * k_Q2GreenRange);
    } else if (T < k_Q3Threshold) {
      const auto S = (T - k_Q2Threshold) / k_Q2Threshold;
      R = static_cast<std::uint8_t>(k_FullChannel);
      G = static_cast<std::uint8_t>(k_Q3GreenBase + (S * k_Q3GreenRange));
    } else {
      const auto S = (T - k_Q3Threshold) / k_Q1Threshold;
      R = static_cast<std::uint8_t>(k_FullChannel);
      G = static_cast<std::uint8_t>(k_Q4GreenBase + (S * k_Q4GreenRange));
      B = static_cast<std::uint8_t>(S * k_Q4BlueRange);
    }

    m_Palette[I] = ftxui::Color::RGB(R, G, B);
  }
}

} // namespace ipm::ui::animations
