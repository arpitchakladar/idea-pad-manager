#include "ui/App.hpp"

#include "ui/NavigatorTab.hpp"
#include "ui/pages/AboutSystem.hpp"
#include "ui/pages/PowerInformation.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui {
App::App()
  : m_FrameRefresher(m_Screen),
    m_Screen(ftxui::ScreenInteractive::Fullscreen()) {}

auto App::setup() -> void {
  const auto NavigatorTab =
    NavigatorTab::create({ "Power Information", "About System" });
  const auto PowerInformation = pages::PowerInformation::create();
  auto AboutSystem = pages::AboutSystem::create();
  const auto Container = ftxui::Container::Vertical({
    NavigatorTab->component(),
    ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }),
    (PowerInformation->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 0;
    })),
    (AboutSystem->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 1;
    })),
  });

  m_App = ftxui::Renderer(Container,
            [&, NavigatorTab, PowerInformation, Container]() -> ftxui::Element {
              auto CurrentFramesPerSecond = 0;
              switch (NavigatorTab->tabNumber()) {
              case 0:

                CurrentFramesPerSecond =
                  PowerInformation->canvasUpdatesPerSecond();
                break;
              case 1:

                CurrentFramesPerSecond = AboutSystem->canvasUpdatesPerSecond();
                break;
              default:
                CurrentFramesPerSecond = 0;
              }

              m_FrameRefresher.setFramesPerSecond(CurrentFramesPerSecond);

              return Container->Render() | ftxui::flex | ftxui::border;
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
