#include <cmath>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/pages/power_information.hpp"
#include "ui/pages/page.hpp"

namespace idea_pad_manager::ui::pages {
	PowerInformation::PowerInformation()
	{
		auto conservation_mode_button_option = ftxui::ButtonOption::Simple();
		conservation_mode_button_option.label = &_conservation_mode_button_message;
		conservation_mode_button_option.on_click = [&] {
			_conservation_mode = !_conservation_mode;
			_conservation_mode_button_message = _conservation_mode ? "ON " : "OFF";
		};
		_conservation_mode_button_message = _conservation_mode ? "ON " : "OFF";
		auto conservation_mode_button = ftxui::Button(conservation_mode_button_option)
			| ftxui::center;
		
		_current_angle = 0.0f;
		
		CreatePage(
			{
				RowDynamic {
					"Fan Speed",
					[] () { return "2100 RPM"; }
				},
				RowStatic {
					"Battery Model Name",
					"L24N4PK3"
				},
				RowStatic {
					"Battery Technology",
					"Li-poly"
				},
				RowDynamic {
					"Battery Capacity",
					[] () { return "76%"; }
				},
				RowStatic {
					"Max battery Capacity",
					"59Wh"
				},
				RowStatic {
					"Manufactured battery Capacity",
					"60Wh"
				},
				RowStatic {
					"Battery charge cycles",
					"11"
				},
				RowDynamic {
					"Battery status",
					[] () { return "Not charging"; }
				},
				RowCustom {
					"Conservation mode",
					conservation_mode_button
				}
			},
			"POWER INFORMATION",
			20,
			[&] () {
				const float rpm = 2000.0f;
				const float speed = (rpm / 1000 / 60.0f) * 2.0f * M_PI;
				
				_current_angle += speed;
				
				if (_current_angle > 2.0f * M_PI)
					_current_angle -= 2.0f * M_PI;
			},
			[&] () {
				const float segments_per_blade = 2.0f;
				const float blade_thickness = 0.5f;
				auto canvas = ftxui::Canvas(100, 100);
				const int center_x = 50, center_y = 50, radius = 40;
				
				for (int i = 0; i < 3; ++i) {
					float base_theta = _current_angle + (i * 2.0f * M_PI / 3.0f);
					for (int j = 0; j < segments_per_blade; ++j) {
						float offset = (static_cast<float>(j) / static_cast<float>(segments_per_blade) - 0.5f) * blade_thickness;
						float theta = base_theta + offset;
						
						int x_end = center_x + static_cast<int>(radius * std::cos(theta));
						int y_end = center_y + static_cast<int>(radius * std::sin(theta));
						
						canvas.DrawPointLine(
							center_x,
							center_y,
							x_end,
							y_end,
							ftxui::Color::Cyan
						);
					}
				}
				
				return canvas;
			}
		);
	}
}
