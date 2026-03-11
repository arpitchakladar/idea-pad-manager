#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_APP_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_APP_HPP

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ui/frame_refresher.hpp"

namespace ipm::ui {
class App {
public:
  App();
  auto setup() -> void;
  auto run() -> void;
  auto stop() -> void;

private:
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_App;
  ftxui::ScreenInteractive m_Screen;
};
} // namespace ipm::ui

#endif
