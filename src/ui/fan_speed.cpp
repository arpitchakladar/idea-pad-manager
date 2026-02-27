#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

#include <ftxui/dom/elements.hpp>
#include <ui/fan_speed.hpp>

namespace UI {
	FanSpeed::FanSpeed() {
		_fanSpeed = ftxui::Renderer([] {
			return ftxui::text("2100 RPM");
		});
	}
}
