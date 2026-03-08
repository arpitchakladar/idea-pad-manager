#include <cmath>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/pages/power_information.hpp"

namespace UI {
	PowerInformation::PowerInformation()
	{
		static float current_angle = 0.0f;
		
		const float target_delta = 0.04;
		const float rpm = 2000.0f;
		const float speed = (rpm / 100 / 60) * 2.0f * M_PI;
		const float segments_per_blade = 10;
		const float blade_thickness = 7;
		
		auto conservationModeButton = ftxui::Button({
			.label = "TOGGLE",
			.on_click = [&] {
				_conservationMode = !_conservationMode;
			},
		});
		
		ftxui::Component powerInformationTable = ftxui::Renderer(
			conservationModeButton,
			[&, conservationModeButton] {
				return ftxui::hbox({
						ftxui::vbox({
							ftxui::text("Fan speed")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Battery model name")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Battery technology")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Battery Capacity")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Max battery Capacity")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Manufactured battery Capacity")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Battery charge cycles")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Battery status")
								| ftxui::color(ftxui::Color::Yellow),
							ftxui::separator(),
							ftxui::text("Conservation mode")
								| ftxui::color(ftxui::Color::Yellow),
						})
							| ftxui::xflex,
						ftxui::separator(),
						ftxui::vbox({
							ftxui::text("2100 RPM"),
							ftxui::separator(),
							ftxui::text("L24N4PK3"),
							ftxui::separator(),
							ftxui::text("Li-poly"),
							ftxui::separator(),
							ftxui::text("76%"),
							ftxui::separator(),
							ftxui::text("59Wh"),
							ftxui::separator(),
							ftxui::text("60Wh"),
							ftxui::separator(),
							ftxui::text("11"),
							ftxui::separator(),
							ftxui::text("Not charging"),
							ftxui::separator(),
							ftxui::hbox({
								ftxui::text(_conservationMode ? "ON " : "OFF ") | ftxui::center,
								conservationModeButton->Render(),
							}) | ftxui::center,
						}) | ftxui::xflex,
					})
					| ftxui::xflex;
			}
		);
		
		CreatePage(
			powerInformationTable,
			"POWER INFORMATION",
			[&, target_delta, speed, segments_per_blade, blade_thickness] (float elapsed_time) {
				if (elapsed_time >= target_delta) {
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
				
				return canvas;
			}
		);
	}
}
