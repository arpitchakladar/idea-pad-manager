#pragma once

#include <chrono>
#include <cstddef>
#include <memory>
#include <random>
#include <vector>

#include "CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class Matrix : public CanvasAnimation {
public:
  Matrix();
  ~Matrix() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  static auto create() -> std::unique_ptr<Matrix> {
    return std::make_unique<Matrix>();
  }
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> int override {
    return k_CanvasUpdatesPerSecond;
  }

private:
  struct Column {
    size_t X;
    size_t Y;
    size_t Speed;
    size_t Length;
  };

  auto initColumn(Column &Col) -> void;

  std::vector<Column> m_Columns;
  std::vector<size_t> m_CharBuffer;

  std::mt19937 m_Rng{ std::random_device{}() };

  std::uniform_int_distribution<int> m_SpeedDist{ k_MinSpeed, k_MaxSpeed };
  std::uniform_int_distribution<int> m_LengthDist{ k_MinLength, k_MaxLength };
  std::uniform_int_distribution<int> m_CharDist{ k_MinChar, k_MaxChar };

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
  size_t m_FrameCount = 0;

  static constexpr auto k_CanvasUpdatesPerSecond = 20;

  static constexpr auto k_MinSpeed = 1;
  static constexpr auto k_MaxSpeed = 5;
  static constexpr auto k_MinLength = 5;
  static constexpr auto k_MaxLength = 20;
  static constexpr auto k_MinChar = 33;
  static constexpr auto k_MaxChar = 126;
  static constexpr auto k_CharRefreshRate = 3;
};

} // namespace ipm::ui::animations
