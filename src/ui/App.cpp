#include "ui/App.hpp"

#include <algorithm>
#include <climits>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

#include "ui/NavigatorTab.hpp"
#include "ui/animations/DoomFire.hpp"
#include "ui/animations/Lightning.hpp"
#include "ui/animations/Matrix.hpp"
#include "ui/pages/AboutSystem.hpp"
#include "ui/pages/PowerInformation.hpp"
#include "ui/pages/ThermalPerformance.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui {
App::App()
  : m_FrameRefresher(m_Screen),
    m_Screen(ftxui::ScreenInteractive::Fullscreen()) {}

auto App::setup() -> void {
  const auto NavigatorTab = NavigatorTab::create(
    { "Power Information", "Thermal & Performance", "About System" });
  const auto PowerInformation = pages::PowerInformation::create();
  auto ThermalPerformance = pages::ThermalPerformance::create();
  auto AboutSystem = pages::AboutSystem::create();

  const auto BackgroundCanvasRenderer = ftxui::Renderer(
    [&, NavigatorTab, PowerInformation, ThermalPerformance, AboutSystem]()
      -> ftxui::Element {
      auto CurrentFramesPerSecond = 0U;
      switch (NavigatorTab->tabNumber()) {
      case 0U:

        CurrentFramesPerSecond = PowerInformation->canvasUpdatesPerSecond();
        break;
      case 1U:

        CurrentFramesPerSecond = ThermalPerformance->canvasUpdatesPerSecond();
        break;
      case 2U:

        CurrentFramesPerSecond = AboutSystem->canvasUpdatesPerSecond();
        break;
      default:
        CurrentFramesPerSecond = 0U;
      }

      const auto ScreenSize = utils::CanvasSize::fullSize();
      auto &Animation = getBackgroundAnimation(NavigatorTab->tabNumber());

      // Resize the canvas on change in screen size
      static auto LastSize = utils::CanvasSize::zero();
      static auto LastTab = UINT_MAX;
      const auto CurrentTab = NavigatorTab->tabNumber();
      if (CurrentTab != LastTab) {
        LastSize = utils::CanvasSize::zero();
        LastTab = CurrentTab;
      }
      if (ScreenSize.Width != LastSize.Width ||
        ScreenSize.Height != LastSize.Height) {
        LastSize = ScreenSize;
        Animation.resize(LastSize);
      }
      CurrentFramesPerSecond =
        std::max(CurrentFramesPerSecond, Animation.canvasUpdatesPerSecond());

      m_FrameRefresher.setFramesPerSecond(CurrentFramesPerSecond);
      if (CurrentFramesPerSecond != 0U) {
        Animation.update();
      }

      const auto Canvas = Animation.drawCanvas();
      return ftxui::canvas(Canvas);
    });

  const auto Container = ftxui::Container::Vertical({
    NavigatorTab->component() | ftxui::clear_under,
    ftxui::Renderer([]() -> ftxui::Element {
      return ftxui::separator() | ftxui::clear_under;
    }),
    (PowerInformation->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 0U;
    })),
    (ThermalPerformance->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 1U;
    })),
    (AboutSystem->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 2U;
    })),
  });

  m_AppCompoment =
    ftxui::Renderer(Container,
      [&, Container, BackgroundCanvasRenderer]() -> ftxui::Element {
        return ftxui::dbox({
                 BackgroundCanvasRenderer->Render(),
                 Container->Render(),
               }) |
          ftxui::clear_under | ftxui::flex | ftxui::border;
      }) |
    ftxui::CatchEvent([&](const ftxui::Event &Event) -> bool {
      if (Event == ftxui::Event::Character('q')) {
        m_Screen.Exit();
        return true;
      }
      return false;
    });
}

auto App::getBackgroundAnimation(uint TabNumber)
  -> animations::CanvasAnimation & {
  switch (TabNumber) {
  case 0U:
    return std::get<0>(m_BackgroundAnimations);
  case 1U:
    return std::get<1>(m_BackgroundAnimations);
  case 2U:
    return std::get<2>(m_BackgroundAnimations);
  default:
    return std::get<std::tuple_size_v<decltype(m_BackgroundAnimations)> - 1>(
      m_BackgroundAnimations);
  }
}

auto App::run() -> void {
  m_FrameRefresher.run();
  m_Screen.Loop(m_AppCompoment);
}

auto App::stop() -> void { m_FrameRefresher.stop(); }
} // namespace ipm::ui
