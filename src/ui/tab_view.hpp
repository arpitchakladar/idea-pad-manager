#include <memory>
#include <ftxui/component/component.hpp>

#pragma once

namespace UI {
	class TabView {
	public:
		TabView();
		inline ftxui::Component component() { return _tabs; }
		inline int tabNumber() const { return _tabNumber; }
		static inline std::shared_ptr<TabView> Create() { return std::make_shared<TabView>(); }

	private:
		int _tabNumber;
		ftxui::Component _tabs;
	};
}
