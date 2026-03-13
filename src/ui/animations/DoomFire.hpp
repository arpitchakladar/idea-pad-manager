#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include <vector>

#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>

#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class DoomFire {
public:
  auto resize(utils::CanvasSize CanvasSize) -> void;
  auto update() -> void;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas;

private:
  auto seedBottomRow() -> void;
  auto spreadFire(size_t SrcIdx) -> void;
  auto buildPalette() -> void;

  utils::CanvasSize m_CanvasSize;

  std::vector<uint8_t> m_Buffer;
  std::vector<ftxui::Color> m_Palette;

  std::mt19937 m_Rng{ std::random_device{}() };
  std::uniform_int_distribution<int> m_DecayDist{ 0, 2 };
  std::uniform_int_distribution<int> m_WindDist{ -1, 1 };

  static constexpr uint8_t g_MaxIntensity = 64;

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};

} // namespace ipm::ui::animations
