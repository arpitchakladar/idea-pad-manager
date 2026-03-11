#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_PAGES_POWER_INFORMATION_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_PAGES_POWER_INFORMATION_HPP

#include <string>

#include "ui/pages/page.hpp"

namespace ipm::ui::pages {
class PowerInformation : public Page {
public:
	PowerInformation();
	static auto create() -> std::shared_ptr<PowerInformation> {
		return std::make_shared<PowerInformation>();
	}

private:
	bool m_ConservationMode;
	std::string m_ConservationModeButtonMessage;
	float m_CurrentAngle;
};
} // namespace ipm::ui::pages

#endif
