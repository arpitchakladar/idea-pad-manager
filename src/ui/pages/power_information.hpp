#include <ftxui/component/component_base.hpp>

#pragma once

namespace UI {
	class PowerInformation {
	public:
		PowerInformation();
		inline ftxui::Component component() { return _powerInformation; }
		static inline std::shared_ptr<PowerInformation> Create() {
			return std::make_shared<PowerInformation>();
		}

	private:
		ftxui::Component _powerInformation;
		bool _conservationMode;
	};
}
