#include "ui/App.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

#include "ui/NavigatorTab.hpp"
#include "ui/animations/DoomFire.hpp"
#include "ui/pages/AboutSystem.hpp"
#include "ui/pages/PowerInformation.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui {
App::App()
  : m_FrameRefresher(m_Screen),
    m_Screen(ftxui::ScreenInteractive::Fullscreen()) {}

auto App::setup() -> void {
  static constexpr auto k_BackgroundTimeIncrement = 0.1F;

  const auto NavigatorTab =
    NavigatorTab::create({ "Power Information", "About System" });
  const auto PowerInformation = pages::PowerInformation::create();
  auto AboutSystem = pages::AboutSystem::create();

  m_BackgroundAnimations.push_back(animations::DoomFire::create());
  m_BackgroundAnimations.push_back(animations::DoomFire::create());

  const auto BackgroundCanvasRenderer =
    ftxui::Renderer([&, NavigatorTab]() -> ftxui::Element {
      const auto ScreenSize = utils::CanvasSize::fullSize();
      auto &Animation = m_BackgroundAnimations[NavigatorTab->tabNumber()];

      // Resize the canvas on change in screen size
      static auto LastSize = utils::CanvasSize::zero();
      static auto LastTab = -1;
      const auto CurrentTab = NavigatorTab->tabNumber();
      if (CurrentTab != LastTab) {
        LastSize = utils::CanvasSize::zero();
        LastTab = CurrentTab;
      }
      if (ScreenSize.Width != LastSize.Width ||
        ScreenSize.Height != LastSize.Height) {
        LastSize = ScreenSize;
        Animation->resize(LastSize);
      }

      Animation->update();

      const auto Canvas = Animation->drawCanvas();
      return ftxui::canvas(Canvas);
    });

  const auto Container = ftxui::Container::Vertical({
    NavigatorTab->component() | ftxui::clear_under,
    ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }),
    (PowerInformation->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 0;
    })),
    (AboutSystem->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 1;
    })),
  });

  m_App =
    ftxui::Renderer(Container,
      [&, NavigatorTab, PowerInformation, Container, BackgroundCanvasRenderer]()
        -> ftxui::Element {
        auto CurrentFramesPerSecond = 0;
        switch (NavigatorTab->tabNumber()) {
        case 0:

          CurrentFramesPerSecond = PowerInformation->canvasUpdatesPerSecond();
          break;
        case 1:

          CurrentFramesPerSecond = AboutSystem->canvasUpdatesPerSecond();
          break;
        default:
          CurrentFramesPerSecond = 0;
        }

        m_FrameRefresher.setFramesPerSecond(CurrentFramesPerSecond);

        m_BackgroundTime += k_BackgroundTimeIncrement;

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

auto App::run() -> void {
  m_FrameRefresher.run();
  m_Screen.Loop(m_App);
}

auto App::stop() -> void { m_FrameRefresher.stop(); }
} // namespace ipm::ui
