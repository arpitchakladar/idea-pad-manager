#include "ui/App.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <spdlog/spdlog.h>

#include "ui/NavigatorTab.hpp"
#include "ui/pages/PowerInformation.hpp"

namespace ipm::ui {
App::App()
  : m_FrameRefresher(m_Screen),
    m_Screen(ftxui::ScreenInteractive::Fullscreen()) {}

auto App::setup() -> void {
  auto NavigatorTab =
    NavigatorTab::create({ "Power Information", "Something else" });
  auto PowerInformation = pages::PowerInformation::create();
  auto Container = ftxui::Container::Vertical({
    NavigatorTab->component(),
    ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }),
    (PowerInformation->component() | ftxui::Maybe([NavigatorTab]() -> bool {
      return NavigatorTab->tabNumber() == 0;
    })),
  });

  m_App = ftxui::Renderer(Container,
            [&, NavigatorTab, PowerInformation, Container]() -> ftxui::Element {
              int CurrentFramesPerSecond = 0;
              if (NavigatorTab->tabNumber() == 0) {
                CurrentFramesPerSecond =
                  PowerInformation->canvasUpdatesPerSecond();
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
