#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

#include "ui/tab_view.hpp"

namespace UI {
	TabView::TabView() {
		_tabNumber = 0;
		static std::vector<std::string> labels = {
			"Fan Speed",
			"Stats",
			"Logs",
		};

		auto tabs = ftxui::Toggle(&labels, &_tabNumber);

		_tabs = Renderer(tabs, [tabs] {
			return tabs->Render()
				| ftxui::bgcolor(ftxui::Color::Black);
		});
	}
}
