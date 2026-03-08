#include <atomic>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#pragma once

namespace UI {
	class App {
	public:
		App();
		~App();
		void setup();
		void run();

	private:
		std::atomic<bool> _running;
		std::thread _frameRefresher;
		ftxui::Component _app;
		ftxui::ScreenInteractive _screen;
	};
}
