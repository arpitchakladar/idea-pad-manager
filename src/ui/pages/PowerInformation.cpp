#include "ui/pages/PowerInformation.hpp"

#include "ui/pages/Page.hpp"

#include <cmath>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

namespace {

auto DrawFilledTriangle(ftxui::Canvas &Canvas,
  float X1,
  float Y1,
  float X2,
  float Y2,
  float X3,
  float Y3,
  const ftxui::Color &Color) -> void {
  if (Y1 > Y2) {
    std::swap(X1, X2);
    std::swap(Y1, Y2);
  }
  if (Y1 > Y3) {
    std::swap(X1, X3);
    std::swap(Y1, Y3);
  }
  if (Y2 > Y3) {
    std::swap(X2, X3);
    std::swap(Y2, Y3);
  }

  if (Y1 == Y3)
    return;

  for (auto Y = static_cast<int>(Y1), EndY = static_cast<int>(Y3); Y <= EndY;
    ++Y) {
    const auto IsBottomHalf = static_cast<float>(Y) > Y2 || Y2 == Y1;
    const auto SegmentHeight = IsBottomHalf ? (Y3 - Y2) : (Y2 - Y1);

    const auto Alpha = (static_cast<float>(Y) - Y1) / (Y3 - Y1);
    const auto Beta =
      (static_cast<float>(Y) - (IsBottomHalf ? Y2 : Y1)) / SegmentHeight;

    auto Ax = static_cast<int>(X1 + (X3 - X1) * Alpha);
    auto Bx = static_cast<int>(
      IsBottomHalf ? (X2 + (X3 - X2) * Beta) : (X1 + (X2 - X1) * Beta));

    if (Ax > Bx)
      std::swap(Ax, Bx);

    for (auto X = Ax; X <= Bx; ++X) {
      Canvas.DrawPoint(X, Y, true, Color);
    }
  }
}
} // namespace

namespace ipm::ui::pages {

PowerInformation::PowerInformation() {
  auto ConservationModeButtonOption = ftxui::ButtonOption::Simple();
  ConservationModeButtonOption.label = &m_ConservationModeButtonMessage;
  ConservationModeButtonOption.on_click = [&]() -> void {
    m_ConservationMode = !m_ConservationMode;
    m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  };
  m_ConservationModeButtonMessage = m_ConservationMode ? "ON " : "OFF";
  auto ConservationModeButton =
    ftxui::Button(ConservationModeButtonOption) | ftxui::center;

  m_CurrentAngle = 0.0F;

  createPage(
    { RowDynamic{ "Fan Speed", []() -> const char * { return "2100 RPM"; } },
      RowStatic{ "Battery Model Name", "L24N4PK3" },
      RowStatic{ "Battery Technology", "Li-poly" },
      RowDynamic{ "Battery Capacity", []() -> const char * { return "76%"; } },
      RowStatic{ "Max battery Capacity", "59Wh" },
      RowStatic{ "Manufactured battery Capacity", "60Wh" },
      RowStatic{ "Battery charge cycles", "11" },
      RowDynamic{
        "Battery status", []() -> const char * { return "Not charging"; } },
      RowCustom{ "Conservation mode", ConservationModeButton } },
    "POWER INFORMATION",
    20,
    [&]() -> void {
      constexpr auto Rpm = 2000.0F;
      constexpr auto Speed =
        (Rpm / 1000.0F / 60.0F) * 2.0F * std::numbers::pi_v<float>;

      m_CurrentAngle += Speed;

      if (m_CurrentAngle > 2.0f * M_PI) {
        m_CurrentAngle -= 2.0F * M_PI;
      }
    },
    [&]() -> ftxui::Canvas {
      constexpr auto PI = std::numbers::pi_v<float>;
      constexpr auto BladeThickness = PI / 24.0F;
      auto Canvas = ftxui::Canvas(100, 100);
      constexpr auto CenterX = 50;
      constexpr auto CenterY = 50;
      constexpr auto Radius = 40.0F;

      for (auto I = 0; I < 3; ++I) {
        const auto BaseTheta =
          m_CurrentAngle + (static_cast<float>(I) * 2.0F * PI / 3.0F);
        const auto Theta = std::make_pair(
          BaseTheta - BladeThickness, BaseTheta + BladeThickness);
        DrawFilledTriangle(Canvas,
          50,
          50,
          CenterX + Radius * std::cos(Theta.first),
          CenterY + Radius * std::sin(Theta.first),
          CenterX + Radius * std::cos(Theta.second),
          CenterY + Radius * std::sin(Theta.second),
          ftxui::Color::Blue);
      }

      return Canvas;
    });
}
} // namespace ipm::ui::pages
