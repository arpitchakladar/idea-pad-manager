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
  static constexpr auto k_MinDecay = 0;
  static constexpr auto k_MaxDecay = 2;
  static constexpr auto k_MinWind = -1;
  static constexpr auto k_MaxWind = 1;
  static constexpr auto k_MaxIntensity = uint8_t(64);

  std::uniform_int_distribution<int> m_DecayDist{ k_MinDecay, k_MaxDecay };
  std::uniform_int_distribution<int> m_WindDist{ k_MinWind, k_MaxWind };

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};

} // namespace ipm::ui::animations
