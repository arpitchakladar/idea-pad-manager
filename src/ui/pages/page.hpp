#include <chrono>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <functional>
#include <string>
#include <tuple>

#pragma once

namespace idea_pad_manager::ui::pages {
	using RowStatic = std::tuple<
			std::string,
			std::string
		>;
	using RowDynamic = std::tuple<
			std::string,
			std::function<std::string()>
		>;
	using RowCustom = std::tuple<
			std::string,
			std::function<std::string()>,
			ftxui::Component
		>;
	
	class Page {
	public:
		inline ftxui::Component component() { return _pageComponent; }
		inline int canvasUpdatesPerSecond() { return _canvasUpdatesPerSecond; }
	
	protected:
		Page() = default;
		void CreatePage(
			std::initializer_list<
				std::variant<
					RowStatic,
					RowDynamic,
					RowCustom
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
