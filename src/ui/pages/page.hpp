#include <chrono>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <functional>

#pragma once

namespace UI {
	class Page {
	public:
		Page(
			ftxui::Component infoTable,
			std::string title,
			std::function<ftxui::Canvas(float)> drawFrame
		);
		inline ftxui::Component component() { return _pageComponent; }
		static inline std::shared_ptr<Page> Create(
			ftxui::Component infoTable,
			std::string title,
			std::function<ftxui::Canvas(float)> drawFrame
		) {
			return std::make_shared<Page>(infoTable, title, drawFrame);
		}

	private:
		ftxui::Component _pageComponent;
		std::chrono::time_point<std::chrono::steady_clock> _lastTime;
		bool _conservationMode;
	};
}
