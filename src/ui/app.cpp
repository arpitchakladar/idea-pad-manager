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
		: _frame_refresher(_screen),
			_screen(ftxui::ScreenInteractive::Fullscreen())
	{}
	
	void App::setup() {
		auto navigator_tab = NavigatorTab::Create({
			"Power Information",
			"Something else"
		});
		auto power_information = pages::PowerInformation::Create();
		auto container = ftxui::Container::Vertical({
			navigator_tab->component(),
			ftxui::Renderer([] { return ftxui::separator(); }),
			(power_information->component()
				| ftxui::Maybe([navigator_tab] { return navigator_tab->tab_number() == 0; })),
		});
		
		_app = ftxui::Renderer(
			container,
			[&, navigator_tab, power_information, container]
			{
				int current_frames_per_second = 0;
				if (navigator_tab->tab_number() == 0) {
					current_frames_per_second = power_information->canvas_updates_per_second();
				}
				
				_frame_refresher.set_frames_per_second(current_frames_per_second);
				
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
		_frame_refresher.run();
		_screen.Loop(_app);
	}
	
	App::~App() {
		_frame_refresher.stop();
	}
}
