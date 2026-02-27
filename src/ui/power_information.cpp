#include <cmath>
#include <chrono>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>

#include "ui/power_information.hpp"

namespace UI {
	PowerInformation::PowerInformation() {
		static auto last_time = std::chrono::steady_clock::now();
		static float current_angle = 0.0f;
		
		const float target_delta = 0.04;
		const float rpm = 2100.0f;
		const float speed = (rpm / 100 / 60) * 2.0f * M_PI;

		_powerInformation = ftxui::Renderer([&, target_delta, speed] {
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
				const float theta = current_angle + (i * 2.0f * M_PI / 3.0f);
				const int x_end = center_x + static_cast<int>(radius * std::cos(theta));
				const int y_end = center_y + static_cast<int>(radius * std::sin(theta));
				canvas.DrawPointLine(center_x, center_y, x_end, y_end, ftxui::Color::Cyan);
			}

			return ftxui::vbox({
				ftxui::text("2100 RPM") | ftxui::hcenter,
				ftxui::canvas(std::move(canvas)) | ftxui::center
			});
		});
	}
}
