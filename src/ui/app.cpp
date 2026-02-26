#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

#include <ui/app.hpp>

App::App()
	: _screen(ftxui::ScreenInteractive::Fullscreen())
{}

void App::setup() {
	auto button = ftxui::Button("Click me", [] {}, ftxui::ButtonOption::Border());
	_app = ftxui::Renderer(button, [button] {
		bool focused = button->Focused();
		return button->Render()
			| ftxui::bgcolor(focused ? ftxui::Color::White : ftxui::Color::Black)
			| ftxui::color(focused ? ftxui::Color::Black : ftxui::Color::White);
	});
}

void App::run() {
	_screen.Loop(_app);
}
