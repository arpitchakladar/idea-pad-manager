#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <spdlog/spdlog.h>

#include "ui/app.hpp"
#include "ui/navigator_tab.hpp"
#include "ui/pages/power_information.hpp"

namespace idea_pad_manager::ui {
	App::App()
		: _frameRefresher(_screen),
			_screen(ftxui::ScreenInteractive::Fullscreen())
	{}
	
	void App::setup() {
		auto navigator_tab = NavigatorTab::Create({
			"Power Information",
			"Something else"
		});
		auto power_information = PowerInformationPage::Create();
		auto container = ftxui::Container::Vertical({
			navigator_tab->component(),
			ftxui::Renderer([] { return ftxui::separator(); }),
			(power_information->component()
				| ftxui::Maybe([navigator_tab] { return navigator_tab->tabNumber() == 0; })),
		});
		
		_app = ftxui::Renderer(
			container,
			[&, navigator_tab, power_information, container]
			{
				int currentFramesPerSecond = 0;
				if (navigator_tab->tabNumber() == 0) {
					currentFramesPerSecond = power_information->canvasUpdatesPerSecond();
				}
				
				_frameRefresher.setFramesPerSecond(currentFramesPerSecond);
				
				return container->Render()
					| ftxui::flex
					| ftxui::border;
			})
				| ftxui::CatchEvent([&](ftxui::Event event) {
					if (event == ftxui::Event::Character('q')) {
						_screen.Exit();
						return true;
					}
					return false;
				});
	}
	
	void App::run() {
		_frameRefresher.run();
		_screen.Loop(_app);
	}
	
	App::~App() {
		_frameRefresher.stop();
	}
}
