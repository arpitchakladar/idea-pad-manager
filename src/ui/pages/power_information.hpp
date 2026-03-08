#include <ftxui/component/component_base.hpp>

#include "ui/pages/page.hpp"

#pragma once

namespace UI {
	class PowerInformationPage: public Page {
	public:
		PowerInformationPage();
		static inline std::shared_ptr<PowerInformationPage> Create() {
			return std::make_shared<PowerInformationPage>();
		}

	private:
		bool _conservationMode;
		float _currentAngle;
	};
}
