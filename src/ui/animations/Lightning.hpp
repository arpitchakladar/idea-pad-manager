#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <random>
#include <utility>
#include <vector>

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
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> int override {
    return k_CanvasUpdatesPerSecond;
  }

private:
  struct Bolt {
    std::vector<std::pair<int, int>> Points;
    uint8_t Intensity{};
    int Life{};
  };

  auto createBolt() -> Bolt;
  auto createBranch(const std::pair<int, int> &Start)
    -> std::vector<std::pair<int, int>>;

  std::vector<Bolt> m_Bolts;
  std::vector<uint8_t> m_Buffer;

  std::mt19937 m_Rng{ std::random_device{}() };

  std::uniform_int_distribution<int> m_XDist{ 0, k_XDistMax };
  std::uniform_int_distribution<int> m_BranchDist{ 0, k_BranchDistMax };
  std::uniform_int_distribution<int> m_LifeDist{ k_MinLife, k_MaxLife };
  std::uniform_int_distribution<int> m_StrikeDist{ 0, k_StrikeDistMax };

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
};

} // namespace ipm::ui::animations
