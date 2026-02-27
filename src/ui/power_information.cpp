#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/power_information.hpp"

namespace UI {
	PowerInformation::PowerInformation() {
		_fanSpeed = ftxui::Renderer([] {
			return ftxui::text("2100 RPM");
		});
	}
}
