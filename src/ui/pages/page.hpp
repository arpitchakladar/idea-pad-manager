#include <chrono>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <functional>

#pragma once

namespace idea_pad_manager::ui::pages {
	class Page {
	public:
		inline ftxui::Component component() { return _pageComponent; }
		inline int canvasUpdatesPerSecond() { return _canvasUpdatesPerSecond; }

	protected:
		Page() = default;
		void CreatePage(
			std::initializer_list<
				std::pair<
					std::string,
					std::variant<
						std::string,
						std::function<std::string()>,
						std::pair<
							std::function<std::string()>,
							ftxui::Component
						>
					>
				>
			> rows,
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
