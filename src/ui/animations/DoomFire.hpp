#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include <ftxui/screen/color.hpp>

#include "CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class DoomFire : public CanvasAnimation {
public:
  DoomFire();
  ~DoomFire() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  static auto create() -> std::unique_ptr<DoomFire> {
    return std::make_unique<DoomFire>();
  }

private:
  auto seedBottomRow() -> void;
  auto spreadFire(size_t SrcIdx) -> void;
  auto buildPalette() -> void;

  std::vector<uint8_t> m_Buffer;
  std::vector<ftxui::Color> m_Palette;

  std::mt19937 m_Rng{ std::random_device{}() };
  static constexpr int g_MinDecay = 0;
  static constexpr int g_MaxDecay = 2;
  static constexpr int g_MinWind = -1;
  static constexpr int g_MaxWind = 1;
  static constexpr uint8_t g_MaxIntensity = 64;

  std::uniform_int_distribution<int> m_DecayDist{ g_MinDecay, g_MaxDecay };
  std::uniform_int_distribution<int> m_WindDist{ g_MinWind, g_MaxWind };

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};

} // namespace ipm::ui::animations
