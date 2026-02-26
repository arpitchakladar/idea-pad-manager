#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>

#include <ftxui/dom/elements.hpp>
#include <ui/app.hpp>
#include <ui/tab_view.hpp>

App::App()
	: _screen(ftxui::ScreenInteractive::Fullscreen())
{}

void App::setup() {
	auto tab_view = TabView::Create();

	_app = ftxui::Renderer(tab_view->component(), [tab_view] {
		bool focused = tab_view->component()->Focused();
		return ftxui::vbox({
			tab_view->component()->Render(),
		}) | ftxui::flex;
	});
}

void App::run() {
	_screen.Loop(_app);
}
