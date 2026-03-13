#include "ui/animations/DoomFire.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <algorithm>

#include <cstddef>
#include <cstdint>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/pixel.hpp>
#include <utility>

namespace ipm::ui::animations {

auto DoomFire::resize(utils::CanvasSize CanvasSize) -> void {
  m_CanvasSize = CanvasSize;
  m_Buffer.assign(m_CanvasSize.Width * m_CanvasSize.Height, 0);
  buildPalette();
  seedBottomRow();
}

auto DoomFire::update() -> void {
  if (m_Buffer.empty()) {
    return;
  }
  for (auto X = 0UL; X < m_CanvasSize.Width; ++X) {
    for (auto Y = 1UL; Y < m_CanvasSize.Height; ++Y) {
      spreadFire((Y * m_CanvasSize.Width) + X);
    }
  }
}

auto DoomFire::draw(ftxui::Canvas &Canvas) const -> void {
  if (m_Buffer.empty()) {
    return;
  }
  for (auto X = 0UL; X < m_CanvasSize.Width; ++X) {
    for (auto Y = 0UL; Y < m_CanvasSize.Height; ++Y) {
      const auto Intensity =
        m_Buffer[static_cast<size_t>((Y * m_CanvasSize.Width) + X)];
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
}

auto DoomFire::seedBottomRow() -> void {
  for (auto X = 0UL; X < m_CanvasSize.Width; ++X) {
    m_Buffer[((m_CanvasSize.Height - 1) * m_CanvasSize.Width) + X] =
      g_MaxIntensity;
  }
}

auto DoomFire::spreadFire(size_t SrcIdx) -> void {
  const auto SrcIntensity = m_Buffer[SrcIdx];

  if (SrcIntensity == 0) {
    m_Buffer[SrcIdx - m_CanvasSize.Width] = 0;
    return;
  }

  const auto Decay = m_DecayDist(m_Rng);
  const auto Wind = m_WindDist(m_Rng);

  const auto DstX = (SrcIdx % m_CanvasSize.Width) + Wind;
  const auto DstY = (SrcIdx / m_CanvasSize.Width) - 1;

  if (DstX < 0 || DstX >= m_CanvasSize.Width) {
    return;
  }

  const auto DstIdx = static_cast<size_t>((DstY * m_CanvasSize.Width) + DstX);
  const auto NewIntensity =
    static_cast<uint8_t>(std::max(0, static_cast<int>(SrcIntensity) - Decay));

  m_Buffer[DstIdx] = NewIntensity;
}

auto DoomFire::buildPalette() -> void {
  m_Palette.resize(g_MaxIntensity + 1);
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

  for (auto I = 1; std::cmp_less_equal(I, g_MaxIntensity); ++I) {
    const auto T = static_cast<float>(I) / static_cast<float>(g_MaxIntensity);
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;

    if (T < k_Q1Threshold) {
      R = static_cast<uint8_t>(T / k_Q1Threshold * k_Q1MaxRed);
    } else if (T < k_Q2Threshold) {
      const auto S = (T - k_Q1Threshold) / k_Q1Threshold;
      R = static_cast<uint8_t>(k_Q2RedBase + (S * k_Q2RedRange));
      G = static_cast<uint8_t>(S * k_Q2GreenRange);
    } else if (T < k_Q3Threshold) {
      const auto S = (T - k_Q2Threshold) / k_Q2Threshold;
      R = static_cast<uint8_t>(k_FullChannel);
      G = static_cast<uint8_t>(k_Q3GreenBase + (S * k_Q3GreenRange));
    } else {
      const auto S = (T - k_Q3Threshold) / k_Q1Threshold;
      R = static_cast<uint8_t>(k_FullChannel);
      G = static_cast<uint8_t>(k_Q4GreenBase + (S * k_Q4GreenRange));
      B = static_cast<uint8_t>(S * k_Q4BlueRange);
    }

    m_Palette[I] = ftxui::Color::RGB(R, G, B);
  }
}

} // namespace ipm::ui::animations
