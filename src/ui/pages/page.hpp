#include <chrono>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>

#pragma once

namespace UI {
	class Page {
	public:
		inline ftxui::Component component() { return _pageComponent; }

	protected:
		Page() = default;
		void CreatePage(
			ftxui::Component infoTable,
			std::string title,
			std::function<ftxui::Canvas(float)> drawFrame
		);

	private:
		ftxui::Component _pageComponent;
		std::chrono::time_point<std::chrono::steady_clock> _lastTime;
		bool _conservationMode;
	};
}
