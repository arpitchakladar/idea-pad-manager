#pragma once

#include "ui/FrameRefresher.hpp"
#include "ui/animations/DoomFire.hpp"

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

namespace ipm::ui {
class App {
public:
  App();
  auto setup() -> void;
  auto run() -> void;
  auto stop() -> void;

private:
  animations::DoomFire m_DoomFire;
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_App;
  ftxui::ScreenInteractive m_Screen;
  float m_BackgroundTime;
};
} // namespace ipm::ui
