#include <cmath>
#include <chrono>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/power_information.hpp"

namespace UI {
	PowerInformation::PowerInformation() {
		static auto last_time = std::chrono::steady_clock::now();
		static float current_angle = 0.0f;

		const float target_delta = 0.04;
		const float rpm = 2000.0f;
		const float speed = (rpm / 100 / 60) * 2.0f * M_PI;
		const float segments_per_blade = 10;
		const float blade_thickness = 7;

		_powerInformation = ftxui::Renderer([&, target_delta, speed, segments_per_blade, blade_thickness] {
			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<float> elapsed = now - last_time;
			const auto elapsed_time = elapsed.count();

			if (elapsed_time >= target_delta) {
				last_time = now;
				current_angle += speed * target_delta;

				if (current_angle > 2.0f * M_PI) current_angle -= 2.0f * M_PI;
			}

			auto canvas = ftxui::Canvas(100, 100);
			const int center_x = 50, center_y = 50, radius = 40;
			
			for (int i = 0; i < 3; ++i) {
				float base_theta = current_angle + (i * 2.0f * M_PI / 3.0f);
				for (int j = 0; j < segments_per_blade; ++j) {
					float offset = (float(j) / segments_per_blade) * blade_thickness - (blade_thickness / 2.0f);
					float theta = base_theta + offset;

					int x_end = center_x + static_cast<int>(radius * std::cos(theta));
					int y_end = center_y + static_cast<int>(radius * std::sin(theta));
					
					canvas.DrawPointLine(center_x, center_y, x_end, y_end, ftxui::Color::Cyan);
				}
			}

			return ftxui::vbox({
					ftxui::filler(),
					ftxui::hbox({
						ftxui::vbox({
							ftxui::text("Power information"),
							ftxui::separator(),
							ftxui::text("Fan speed: 2100 RPM"),
						})
							| ftxui::xflex,
						ftxui::separator(),
						ftxui::canvas(std::move(canvas)) | ftxui::center,
					})
						| ftxui::borderRounded
						| ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 100)
						| ftxui::center,
					ftxui::filler(),
			}) | ftxui::yflex;
		});
	}
}
