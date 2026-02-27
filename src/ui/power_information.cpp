#include <cmath>
#include <thread>
#include <atomic>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>

#include "ui/power_information.hpp"

namespace UI {
	PowerInformation::PowerInformation() {
		static float current_angle = 0.0f;

		_powerInformation = ftxui::Renderer([&] {
			auto canvas = ftxui::Canvas(100, 100);
			
			int center_x = 50;
			int center_y = 50;
			int radius = 40;
			
			for (int i = 0; i < 3; ++i) {
				float theta = current_angle + (i * 2.0f * M_PI / 3.0f);
				int x_end = center_x + static_cast<int>(radius * std::cos(theta));
				int y_end = center_y + static_cast<int>(radius * std::sin(theta));
				
				canvas.DrawPointLine(center_x, center_y, x_end, y_end, ftxui::Color::Cyan);
			}

			current_angle += 0.15f;

			return ftxui::vbox({
				ftxui::text("2100 RPM") | ftxui::hcenter,
				ftxui::canvas(std::move(canvas))
					| ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 100)
					| ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 100)
					| ftxui::center
			});
		});
	}
}
