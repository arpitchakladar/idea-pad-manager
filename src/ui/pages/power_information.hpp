#include <ftxui/component/component_base.hpp>

#include "ui/pages/page.hpp"

#pragma once

namespace UI {
	class PowerInformation: public Page {
	public:
		PowerInformation();
		static inline std::shared_ptr<PowerInformation> Create() {
			return std::make_shared<PowerInformation>();
		}

	private:
		bool _conservationMode;
	};
}
