#include "ui/animations/Lightning.hpp"
#include "ui/animations/CanvasAnimation.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <algorithm>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/pixel.hpp>

namespace ipm::ui::animations {

namespace {
const ftxui::Color g_LightningWhite = ftxui::Color::RGB(255, 255, 255);
const ftxui::Color g_LightningBlue = ftxui::Color::RGB(100, 150, 255);
const ftxui::Color g_LightningDim = ftxui::Color::RGB(50, 80, 150);
const ftxui::Color g_BuildingColor = ftxui::Color::RGB(20, 20, 30);
const ftxui::Color g_WindowLit = ftxui::Color::RGB(255, 230, 100);
const ftxui::Color g_WindowDark = ftxui::Color::RGB(40, 40, 60);
} // namespace

Lightning::Lightning() { m_LastTime = std::chrono::steady_clock::now(); }

auto Lightning::resize(utils::CanvasSize CanvasSize) -> void {
  CanvasAnimation::resize(CanvasSize);
  m_Buffer.assign(CanvasSize.Width * CanvasSize.Height, 0);
  m_Bolts.clear();
  generateBuildings();
}

auto Lightning::update() -> void {
  if (m_Buffer.empty()) {
    return;
  }
  static constexpr auto k_Delta =
    1.0F / static_cast<float>(k_CanvasUpdatesPerSecond);
  const auto Now = std::chrono::steady_clock::now();
  const auto ElapsedTime =
    std::chrono::duration<float>(Now - m_LastTime).count();
  if (ElapsedTime < k_Delta) {
    return;
  }

  const auto CanvasSize = canvasSize();
  m_FrameCount++;

  for (auto &Bolt : m_Bolts) {
    Bolt.Life--;
  }
  std::erase_if(m_Bolts, [](const Bolt &B) { return B.Life <= 0; });

  const auto StrikeRoll = m_StrikeDist(m_Rng);
  if (StrikeRoll < k_StrikeChance && m_Bolts.size() < k_MaxBolts) {
    m_Bolts.push_back(createBolt());
  }

  m_Buffer.assign(m_Buffer.size(), 0);
  for (const auto &Bolt : m_Bolts) {
    const auto Alpha = static_cast<uint8_t>(std::min<int>(
      k_MaxIntensity, static_cast<int>(Bolt.Intensity) * Bolt.Life));
    for (const auto &Point : Bolt.Points) {
      if (Point.first >= 0 &&
        Point.first < static_cast<int>(CanvasSize.Width) && Point.second >= 0 &&
        Point.second < static_cast<int>(CanvasSize.Height)) {
        const auto Idx = static_cast<std::size_t>(
          (Point.second * CanvasSize.Width) + Point.first);
        m_Buffer[Idx] = std::max(m_Buffer[Idx], Alpha);
      }
    }
  }

  m_LastTime = Now;
}

auto Lightning::drawCanvas() const -> utils::CustomCanvas {
  const auto CanvasSize = canvasSize();
  auto Canvas = utils::CustomCanvas(CanvasSize);
  if (!m_Buffer.empty()) {
    drawLightning(Canvas);
  }
  drawBuildings(Canvas);
  return Canvas;
}

auto Lightning::drawLightning(utils::CustomCanvas &Canvas) const -> void {
  const auto CanvasSize = canvasSize();
  for (auto X = 0UL; X < CanvasSize.Width; ++X) {
    for (auto Y = 0UL; Y < CanvasSize.Height; ++Y) {
      const auto Intensity =
        m_Buffer[static_cast<std::size_t>((Y * CanvasSize.Width) + X)];
      if (Intensity == 0) {
        continue;
      }
      const auto Color = getLightningColor(Intensity);
      Canvas.DrawBlock(static_cast<int>(X),
        static_cast<int>(Y),
        true,
        [Color](ftxui::Pixel &P) {
          P.background_color = Color;
          P.foreground_color = Color;
        });
    }
  }
}

auto Lightning::getLightningColor(uint8_t Intensity) -> ftxui::Color {
  const float T =
    static_cast<float>(Intensity) / static_cast<float>(k_MaxIntensity);
  if (T > k_IntensityThresholdWhite) {
    return g_LightningWhite;
  }
  if (T > k_IntensityThresholdBlue) {
    return g_LightningBlue;
  }
  return g_LightningDim;
}

auto Lightning::drawBuildings(utils::CustomCanvas &Canvas) const -> void {
  const auto CanvasSize = canvasSize();
  for (const auto &B : m_Buildings) {
    drawBuilding(Canvas, B, CanvasSize);
  }
}

auto Lightning::drawBuilding(utils::CustomCanvas &Canvas,
  const Building &B,
  const utils::CanvasSize &CanvasSize) const -> void {
  const auto BuildingTop = static_cast<int>(CanvasSize.Height) - B.Height;
  for (auto Bx = B.X;
    Bx < B.X + B.Width && Bx < static_cast<int>(CanvasSize.Width);
    ++Bx) {
    for (auto By = BuildingTop; By < static_cast<int>(CanvasSize.Height);
      ++By) {
      Canvas.DrawBlock(Bx, By, true, [](ftxui::Pixel &P) {
        P.background_color = g_BuildingColor;
        P.foreground_color = g_BuildingColor;
      });
    }
  }
  drawWindows(Canvas, B, BuildingTop, CanvasSize);
}

auto Lightning::drawWindows(utils::CustomCanvas &Canvas,
  const Building &B,
  int BuildingTop,
  const utils::CanvasSize &CanvasSize) const -> void {
  if (B.WindowRows == 0 || B.WindowCols == 0) {
    return;
  }
  const auto WindowWidth = B.Width / B.WindowCols;
  const auto WindowHeight = B.Height / B.WindowRows;
  for (auto Row = 0; Row < B.WindowRows; ++Row) {
    for (auto Col = 0; Col < B.WindowCols; ++Col) {
      const auto WindowX = B.X + (Col * WindowWidth) + (WindowWidth / 2);
      const auto WindowY =
        BuildingTop + (Row * WindowHeight) + (WindowHeight / 2);
      if (WindowX >= 0 && WindowX < static_cast<int>(CanvasSize.Width) &&
        WindowY >= 0 && WindowY < static_cast<int>(CanvasSize.Height)) {
        const bool IsLit =
          (m_FrameCount / k_WindowFlickerRate + Row + Col) % 3 != 0;
        const auto &WindowColor = IsLit ? g_WindowLit : g_WindowDark;
        Canvas.DrawBlock(
          WindowX, WindowY, true, [WindowColor](ftxui::Pixel &P) {
            P.background_color = WindowColor;
            P.foreground_color = WindowColor;
          });
      }
    }
  }
}

auto Lightning::createBolt() -> Bolt {
  const auto CanvasSize = canvasSize();
  Bolt NewBolt;
  NewBolt.Intensity = k_MaxIntensity;
  NewBolt.Life = m_LifeDist(m_Rng);

  auto X = static_cast<int>(m_XDist(m_Rng) * CanvasSize.Width / k_XDistMax);
  auto Y = 0;

  NewBolt.Points.emplace_back(X, Y);

  while (Y < static_cast<int>(CanvasSize.Height)) {
    const auto NextX = X + (static_cast<int>(m_Rng()) % k_BoltXVariation) - 1;
    Y +=
      (static_cast<int>(m_Rng()) % k_BoltYVariationMax) + k_BoltYVariationMin;

    if (NextX >= 0 && NextX < static_cast<int>(CanvasSize.Width)) {
      X = NextX;
    }

    NewBolt.Points.emplace_back(X, Y);

    const auto BranchRoll = m_BranchDist(m_Rng);
    if (BranchRoll < k_BranchChance &&
      Y < static_cast<int>(CanvasSize.Height) - k_BranchYThreshold) {
      const auto Branch = createBranch(NewBolt.Points.back());
      for (const auto &P : Branch) {
        if (P.second < static_cast<int>(CanvasSize.Height)) {
          NewBolt.Points.push_back(P);
        }
      }
    }
  }

  return NewBolt;
}

auto Lightning::createBranch(const std::pair<int, int> &Start)
  -> std::vector<std::pair<int, int>> {
  const auto CanvasSize = canvasSize();
  std::vector<std::pair<int, int>> BranchPoints;

  auto X = Start.first;
  auto Y = Start.second;
  const auto BranchLength =
    (static_cast<int>(m_Rng()) % (k_BranchLengthMax - k_BranchLengthMin + 1)) +
    k_BranchLengthMin;
  const auto Direction = m_Rng() % 2 == 0 ? 1 : -1;

  for (auto I = 0; I < BranchLength; ++I) {
    X += Direction *
      ((static_cast<int>(m_Rng()) % k_BoltYVariationMax) + k_BoltYVariationMin);
    Y +=
      (static_cast<int>(m_Rng()) % k_BoltYVariationMax) + k_BoltYVariationMin;

    if (X < 0 || X >= static_cast<int>(CanvasSize.Width) ||
      Y >= static_cast<int>(CanvasSize.Height)) {
      break;
    }

    BranchPoints.emplace_back(X, Y);
  }

  return BranchPoints;
}

auto Lightning::generateBuildings() -> void {
  const auto CanvasSize = canvasSize();
  m_Buildings.clear();

  auto X = 0;
  while (X < static_cast<int>(CanvasSize.Width)) {
    auto B = Building{
      .X = X,
      .Width = m_BuildingWidthDist(m_Rng),
      .Height = m_BuildingHeightDist(m_Rng),
      .WindowRows = m_BuildingWindowsDist(m_Rng),
      .WindowCols = m_BuildingWindowsDist(m_Rng),
    };
    m_Buildings.push_back(B);
    X += B.Width;
  }
}

} // namespace ipm::ui::animations
