#pragma once

#include "ui/FrameRefresher.hpp"
#include "ui/animations/CanvasAnimation.hpp"
#include "ui/animations/DoomFire.hpp"
#include "ui/animations/Lightning.hpp"
#include "ui/animations/Matrix.hpp"

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <tuple>

namespace ipm::ui {
class App {
public:
  App();
  App(const App &) = delete;
  App &operator=(const App &) = delete;
  App(App &&) = delete;
  App &operator=(App &&) = delete;
  ~App() = default;

  auto setup() -> void;
  auto run() -> void;
  auto stop() -> void;

private:
  std::tuple<animations::Lightning, animations::DoomFire, animations::Matrix>
    m_BackgroundAnimations;
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_App;
  ftxui::ScreenInteractive m_Screen;

  auto getBackgroundAnimation(uint TabNumber) -> animations::CanvasAnimation &;
};
} // namespace ipm::ui
