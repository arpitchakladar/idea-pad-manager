#include <memory>
#include <vector>

#include <ftxui/component/component.hpp>

#pragma once

namespace UI {
	class NavigatorTab {
	public:
		NavigatorTab(std::vector<std::string> labels);
		inline ftxui::Component component() { return _tabs; }
		inline int tabNumber() const { return _tabNumber; }
		static inline std::shared_ptr<NavigatorTab> Create(
			std::vector<std::string> labels
		) {
			return std::make_shared<NavigatorTab>(std::move(labels));
		}
	
	private:
		int _tabNumber;
		std::vector<std::string> _labels;
		ftxui::Component _tabs;
	};
}
