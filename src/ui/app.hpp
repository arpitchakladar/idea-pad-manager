#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_APP_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_APP_HPP

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ui/frame_refresher.hpp"

#pragma once

namespace idea_pad_manager::ui {
class App {
public:
  App();
  void setup();
  void run();
  void stop();

private:
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_App;
  ftxui::ScreenInteractive m_Screen;
};
} // namespace idea_pad_manager::ui

#endif
