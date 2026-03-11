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
			ftxui::Component
		>;
	
	class Page {
	public:
		inline ftxui::Component component() { return _page_component; }
		inline int canvas_updates_per_second() { return _canvas_updates_per_second; }
	
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
			int canvas_updates_per_second,
			std::function<void()> update_canvas,
			std::function<ftxui::Canvas()> draw_canvas
		);

	private:
		ftxui::Component _page_component;
		int _canvas_updates_per_second;
		std::chrono::time_point<std::chrono::steady_clock> _last_time;
	};
}
