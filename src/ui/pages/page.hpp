#include <chrono>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>

#pragma once

namespace UI {
	class Page {
	public:
		inline ftxui::Component component() { return _pageComponent; }
		inline int canvasUpdatesPerSecond() { return _canvasUpdatesPerSecond; }

	protected:
		Page() = default;
		void CreatePage(
			ftxui::Component infoTable,
			std::string title,
			int canvasUpdatesPerSecond,
			std::function<void()> updateCanvas,
			std::function<ftxui::Canvas()> drawCanvas
		);

	private:
		ftxui::Component _pageComponent;
		int _canvasUpdatesPerSecond;
		std::chrono::time_point<std::chrono::steady_clock> _lastTime;
	};
}
