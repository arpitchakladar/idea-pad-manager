#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>

int main(int argc, char *argv[]) {
	auto screen = ftxui::ScreenInteractive::Fullscreen();
	auto button = ftxui::Button("Click me", [] {});

	auto app = ftxui::Renderer(button, [&] {
		bool focused = button->Focused();
		return button->Render()
			| ftxui::bgcolor(focused ? ftxui::Color::Blue : ftxui::Color::Black)
			| ftxui::color(focused ? ftxui::Color::White : ftxui::Color::GrayLight)
			| ftxui::border;
	});

	screen.Loop(app);

	return 0;
}
