#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/navigator_tab.hpp"

namespace UI {
	NavigatorTab::NavigatorTab(
		std::vector<std::string> labels
	) {
		_tabNumber = 0;
		_labels = std::move(labels);
		
		auto tabs = ftxui::Toggle(&_labels, &_tabNumber);
		
		_tabs = ftxui::Renderer(tabs, [tabs] {
			return tabs->Render()
				| ftxui::bgcolor(ftxui::Color::Black)
				| ftxui::center;
		});
	}
}
