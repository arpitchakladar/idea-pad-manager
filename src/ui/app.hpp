#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#pragma once

class App {
public:
	App();
	void setup();
	void run();

private:
	ftxui::Component _app;
	ftxui::ScreenInteractive _screen;
};
