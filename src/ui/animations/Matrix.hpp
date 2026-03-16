#pragma once

#include <chrono>
#include <memory>
#include <random>
#include <vector>

#include "CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::animations {

class Matrix : public CanvasAnimation {
public:
  Matrix();
  Matrix(const Matrix &) = default;
  Matrix &operator=(const Matrix &) = default;
  Matrix(Matrix &&) = default;
  Matrix &operator=(Matrix &&) = default;
  ~Matrix() override = default;

  auto resize(utils::CanvasSize CanvasSize) -> void override;
  auto update() -> void override;
  [[nodiscard]] auto drawCanvas() const -> utils::CustomCanvas override;
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint override {
    return k_CanvasUpdatesPerSecond;
  }

private:
  struct Column {
    uint X;
    uint Y;
    uint Speed;
    uint Length;
  };

  auto initColumn(Column &Col) -> void;

  std::vector<Column> m_Columns;
  std::vector<uint> m_CharBuffer;

  std::mt19937 m_Rng{ std::random_device{}() };

  std::uniform_int_distribution<int> m_SpeedDist{ k_MinSpeed, k_MaxSpeed };
  std::uniform_int_distribution<int> m_LengthDist{ k_MinLength, k_MaxLength };
  std::uniform_int_distribution<int> m_CharDist{ k_MinChar, k_MaxChar };

  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
  uint m_FrameCount = 0;

  static constexpr auto k_CanvasUpdatesPerSecond = 20U;
  static constexpr auto k_MinSpeed = 1U;
  static constexpr auto k_MaxSpeed = 5U;
  static constexpr auto k_MinLength = 5U;
  static constexpr auto k_MaxLength = 20U;
  static constexpr auto k_MinChar = 33U;
  static constexpr auto k_MaxChar = 126U;
  static constexpr auto k_CharRefreshRate = 3U;

  static_assert(k_MaxSpeed >= k_MinSpeed, "Max speed must be >= min speed");
  static_assert(k_MaxLength >= k_MinLength, "Max length must be >= min length");
  static_assert(k_MaxChar >= k_MinChar, "Max char must be >= min char");
  static_assert(k_CharRefreshRate > 0, "Char refresh rate must be positive");
};

} // namespace ipm::ui::animations
