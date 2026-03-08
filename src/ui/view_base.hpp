#include <chrono>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <functional>

#pragma once

namespace UI {
	class ViewBase {
	public:
		ViewBase(ftxui::Component infoTable, std::function<ftxui::Canvas(float)> drawFrame);
		inline ftxui::Component component() { return _viewComponent; }
		static inline std::shared_ptr<ViewBase> Create(
			ftxui::Component infoTable,
			std::function<ftxui::Canvas(float)> drawFrame
		) {
			return std::make_shared<ViewBase>(infoTable, drawFrame);
		}

	private:
		ftxui::Component _viewComponent;
		std::chrono::time_point<std::chrono::steady_clock> _lastTime;
		bool _conservationMode;
	};
}
