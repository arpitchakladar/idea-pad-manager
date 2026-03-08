#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ui/frame_refresher.hpp"

#pragma once

namespace idea_pad_manager::ui {
	class App {
	public:
		App();
		~App();
		void setup();
		void run();
	
	private:
		FrameRefresher _frameRefresher;
		ftxui::Component _app;
		ftxui::ScreenInteractive _screen;
	};
}
