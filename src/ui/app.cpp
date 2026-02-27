#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
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

		_app = ftxui::Renderer(tab_view->component(), [tab_view, power_information] {
			bool focused = tab_view->component()->Focused();
			return ftxui::vbox({
				tab_view->component()->Render(),
				ftxui::separator(),
				(power_information->component()
					| ftxui::Maybe([tab_view] { return tab_view->tabNumber() == 0; })
				)->Render()
			})
				| ftxui::flex
				| ftxui::border;
		});
	}

	void App::run() {
		_screen.Loop(_app);
	}
}
