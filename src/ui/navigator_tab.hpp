#include <memory>
#include <vector>

#include <ftxui/component/component.hpp>

#pragma once

namespace idea_pad_manager::ui {
	class NavigatorTab {
	public:
		NavigatorTab(std::vector<std::string> labels);
		inline ftxui::Component component() { return _tabs; }
		inline int tab_number() const { return _tab_number; }
		static inline std::shared_ptr<NavigatorTab> Create(
			std::vector<std::string> labels
		) {
			return std::make_shared<NavigatorTab>(std::move(labels));
		}
	
	private:
		int _tab_number;
		std::vector<std::string> _labels;
		ftxui::Component _tabs;
	};
}
