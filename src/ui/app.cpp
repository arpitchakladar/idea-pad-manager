#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/app.hpp"
#include "ui/tab_view.hpp"
#include "ui/fan_speed.hpp"

namespace UI {
	App::App()
		: _screen(ftxui::ScreenInteractive::Fullscreen())
	{}

	void App::setup() {
		auto tab_view = TabView::Create();
		auto fan_speed = FanSpeed::Create();

		_app = ftxui::Renderer(tab_view->component(), [tab_view, fan_speed] {
			bool focused = tab_view->component()->Focused();
			return ftxui::vbox({
				tab_view->component()->Render(),
				(fan_speed->component()
					| ftxui::Maybe([tab_view] { return tab_view->tabNumber() == 0; })
				)->Render()
			}) | ftxui::flex;
		});
	}

	void App::run() {
		_screen.Loop(_app);
	}
}
