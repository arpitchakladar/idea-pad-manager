#pragma once

#include <tuple>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ui/FrameRefresher.hpp"
#include "ui/animations/Animation.hpp"
#include "ui/animations/DoomFire.hpp"
#include "ui/animations/Empty.hpp"
#include "ui/animations/Lightning.hpp"
#include "ui/animations/Matrix.hpp"

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
  std::tuple<animations::Lightning,
    animations::DoomFire,
    animations::Matrix,
    animations::Empty>
    m_BackgroundAnimations;
  FrameRefresher m_FrameRefresher;
  ftxui::Component m_AppCompoment;
  ftxui::ScreenInteractive m_Screen;

  auto getBackgroundAnimation(uint TabNumber) -> animations::Animation &;
};
} // namespace ipm::ui
