#include <ftxui/component/component_base.hpp>

#pragma once

namespace UI {
	class FanSpeed {
	public:
		FanSpeed();
		inline ftxui::Component component() { return _fanSpeed; }
		static inline std::shared_ptr<FanSpeed> Create() { return std::make_shared<FanSpeed>(); }

	private:
		ftxui::Component _fanSpeed;
	};
}
