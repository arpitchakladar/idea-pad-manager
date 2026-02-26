#include <ftxui/component/component.hpp>

#include <ftxui/dom/elements.hpp>
#include <memory>
#include <ui/tab_view.hpp>

TabView::TabView() {
	_tabNumber = 0;
	static std::vector<std::string> labels = {
		"Overview",
		"Stats",
		"Logs",
	};

	auto tabs = ftxui::Toggle(&labels, &_tabNumber);

	_tabs = Renderer(tabs, [tabs] {
		return tabs->Render()
			| ftxui::border
			| ftxui::bgcolor(ftxui::Color::Black)
			| ftxui::hcenter;
	});
}
