#pragma once

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ui/FrameRefresher.hpp"

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
