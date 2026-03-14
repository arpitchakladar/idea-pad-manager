#pragma once

#include "ui/FrameRefresher.hpp"
#include "ui/animations/CanvasAnimation.hpp"

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <vector>

namespace ipm::ui {
class App {
public:
  App();
  auto setup() -> void;
  auto run() -> void;
  auto stop() -> void;

private:
  std::vector<std::unique_ptr<animations::CanvasAnimation>>
    m_BackgroundAnimations;
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_App;
  ftxui::ScreenInteractive m_Screen;
  float m_BackgroundTime{ 0.0F };
};
} // namespace ipm::ui
