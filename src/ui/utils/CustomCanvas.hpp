#pragma once

#include <ftxui/dom/canvas.hpp>

namespace ipm::ui::utils {

class CustomCanvas : public ftxui::Canvas {
public:
  CustomCanvas(int Width, int Height);
  auto DrawFilledTriangle(float X1,
    float Y1,
    float X2,
    float Y2,
    float X3,
    float Y3,
    const ftxui::Color &Color) -> void;
};
} // namespace ipm::ui::utils
