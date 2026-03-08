#include <thread>
#include <atomic>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/app.hpp"
#include "ui/tab_view.hpp"
#include "ui/power_information.hpp"

namespace UI {
	App::App()
		: _screen(ftxui::ScreenInteractive::Fullscreen())
	{}

	void App::setup() {
		auto tab_view = TabView::Create();
		auto power_information = PowerInformation::Create();
		auto container = ftxui::Container::Vertical({
			tab_view->component(),
			ftxui::Renderer([] { return ftxui::separator(); }),
			(power_information->component()
				| ftxui::Maybe([tab_view] { return tab_view->tabNumber() == 0; })),
		});

		_app = ftxui::Renderer(container, [tab_view, power_information, container] {
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
		_running = true;
		_frameRefresher = std::thread([&] {
			while (_running) {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				_screen.PostEvent(ftxui::Event::Custom);
			}
		});
		_screen.Loop(_app);
	}

	App::~App() {
		_running = false;
		_frameRefresher.join();
	}
}
