#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <random>
#include <utility>
#include <vector>

#include <ftxui/screen/color.hpp>

#include "CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class Lightning : public CanvasAnimation {
public:
  Lightning();
  Lightning(const Lightning &) = default;
  Lightning &operator=(const Lightning &) = default;
  Lightning(Lightning &&) = default;
  Lightning &operator=(Lightning &&) = default;
  ~Lightning() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  static auto create() -> std::unique_ptr<Lightning> {
    return std::make_unique<Lightning>();
  }
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_CanvasUpdatesPerSecond;
  }

private:
  struct Bolt {
    std::vector<std::pair<int, int>> Points;
    uint8_t Intensity;
    int Life;
  };

  struct Building {
    uint X;
    uint Width;
    uint Height;
    uint WindowRows;
    uint WindowCols;
  };

  auto createBolt() -> Bolt;
  auto createBranch(const std::pair<int, int> &Start)
    -> std::vector<std::pair<int, int>>;
  auto generateBuildings() -> void;

  auto drawLightning(utils::CustomCanvas &Canvas) const -> void;
  static auto getLightningColor(uint8_t Intensity) -> ftxui::Color;
  auto drawBuildings(utils::CustomCanvas &Canvas) const -> void;
  auto drawBuilding(utils::CustomCanvas &Canvas,
    const Building &B,
    const utils::CanvasSize &CanvasSize) const -> void;
  auto drawWindows(utils::CustomCanvas &Canvas,
    const Building &B,
    uint BuildingTop,
    const utils::CanvasSize &CanvasSize) const -> void;

  std::vector<Bolt> m_Bolts;
  std::vector<uint8_t> m_Buffer;
  std::vector<Building> m_Buildings;

  std::mt19937 m_Rng{ std::random_device{}() };

  std::uniform_int_distribution<int> m_XDist{ 0, k_XDistMax };
  std::uniform_int_distribution<int> m_BranchDist{ 0, k_BranchDistMax };
  std::uniform_int_distribution<int> m_LifeDist{ k_MinLife, k_MaxLife };
  std::uniform_int_distribution<int> m_StrikeDist{ 0, k_StrikeDistMax };
  std::uniform_int_distribution<int> m_BuildingWidthDist{ k_BuildingMinWidth,
    k_BuildingMaxWidth };
  std::uniform_int_distribution<int> m_BuildingHeightDist{ k_BuildingMinHeight,
    k_BuildingMaxHeight };
  std::uniform_int_distribution<int> m_BuildingWindowsDist{
    k_BuildingMinWindows, k_BuildingMaxWindows
  };

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
  int m_FrameCount = 0;

  static constexpr auto k_CanvasUpdatesPerSecond = 30;
  static constexpr auto k_StrikeChance = 30;
  static constexpr auto k_MaxBolts = 15;
  static constexpr auto k_BranchChance = 40;
  static constexpr auto k_MaxIntensity = std::uint8_t(255);

  static constexpr auto k_IntensityThresholdWhite = 0.8F;
  static constexpr auto k_IntensityThresholdBlue = 0.4F;

  static constexpr auto k_XDistMax = 100;
  static constexpr auto k_BranchDistMax = 100;
  static constexpr auto k_MinLife = 3;
  static constexpr auto k_MaxLife = 10;
  static constexpr auto k_StrikeDistMax = 100;

  static constexpr auto k_BoltXVariation = 3;
  static constexpr auto k_BoltYVariationMin = 1;
  static constexpr auto k_BoltYVariationMax = 2;
  static constexpr auto k_BranchYThreshold = 5;
  static constexpr auto k_BranchLengthMin = 3;
  static constexpr auto k_BranchLengthMax = 5;

  static constexpr auto k_BuildingMinWidth = 3;
  static constexpr auto k_BuildingMaxWidth = 8;
  static constexpr auto k_BuildingMinHeight = 20;
  static constexpr auto k_BuildingMaxHeight = 80;
  static constexpr auto k_BuildingMinWindows = 1;
  static constexpr auto k_BuildingMaxWindows = 4;
  static constexpr auto k_WindowFlickerRate = 10;

  static_assert(k_MaxLife >= k_MinLife, "Max life must be >= min life");
  static_assert(k_BoltYVariationMax >= k_BoltYVariationMin,
    "Max bolt Y variation must be >= min");
  static_assert(
    k_BranchLengthMax >= k_BranchLengthMin, "Max branch length must be >= min");
  static_assert(k_BuildingMaxWidth >= k_BuildingMinWidth,
    "Max building width must be >= min");
  static_assert(k_BuildingMaxHeight >= k_BuildingMinHeight,
    "Max building height must be >= min");
  static_assert(k_BuildingMaxWindows >= k_BuildingMinWindows,
    "Max building windows must be >= min");
  static_assert(k_MaxIntensity > 0, "Max intensity must be positive");
  static_assert(
    k_WindowFlickerRate > 0, "Window flicker rate must be positive");
};

} // namespace ipm::ui::animations
