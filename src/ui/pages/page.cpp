#include <algorithm>
#include <chrono>
#include <ftxui/dom/node.hpp>
#include <initializer_list>
#include <string>
#include <variant>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <variant>

#include "ui/pages/page.hpp"

namespace idea_pad_manager::ui::pages {
	void Page::CreatePage(
		std::initializer_list<
			std::variant<
				RowStatic,
				RowDynamic,
				RowCustom
			>
		> rows,
		std::string title,
		int canvas_updates_per_second,
		std::function<void()> update_canvas,
		std::function<ftxui::Canvas()> draw_canvas
	) {
		_canvas_updates_per_second = canvas_updates_per_second;
		_last_time = std::chrono::steady_clock::now();
		
		const float delta = canvas_updates_per_second > 0.0f
			? 1.0f / static_cast<float>(canvas_updates_per_second)
			: -1.0;
		std::vector<ftxui::Element> info_table_labels;
		info_table_labels.reserve(rows.size() * 2 - 1);
		std::vector<ftxui::Component> info_table_values;
		info_table_values.reserve(rows.size() * 2 - 1);
		
		auto last_ptr = rows.end() - 1;
		for (auto it = rows.begin(); it != rows.end(); it++) {
			std::visit(
				[&](auto&& row_data) {
					using T = std::decay_t<decltype(row_data)>;
					
					info_table_labels.push_back(
						ftxui::text(std::move(std::get<0>(row_data)))
							| ftxui::color(ftxui::Color::Yellow)
							| ftxui::vcenter
					);
					
					if constexpr (std::is_same_v<
						T,
						RowCustom
					>) {
						info_table_values.push_back(
							ftxui::Renderer(
								get<1>(row_data),
								[row_data = std::move(row_data)] {
									return get<1>(row_data)->Render();
								}
							)
						);
					} else if constexpr (std::is_same_v<
						T,
						RowDynamic
					>) {
						info_table_values.push_back(
							ftxui::Renderer([row_data = std::move(row_data)] {
								return ftxui::text(get<1>(row_data)());
							})
						);
					} else if constexpr (std::is_same_v<
						T,
						RowStatic
					>) {
						info_table_values.push_back(
							ftxui::Renderer([row_data = std::move(row_data)] {
								return ftxui::text(get<1>(row_data));
							})
						);
					}
				},
				std::move(*it)
			);
			
			if (it != last_ptr) {
				info_table_labels.push_back(ftxui::separator());
				info_table_values.push_back(
					ftxui::Renderer(
						[] () { return ftxui::separator(); }
					)
				);
			}
		}
		
		const auto info_table_valuesComponent = ftxui::Container::Vertical(info_table_values);
		
		_page_component = ftxui::Renderer(
			info_table_valuesComponent,
			[
				&,
				info_table_valuesComponent,
				info_table_labels,
				update_canvas,
				draw_canvas,
				delta,
				title = std::move(title)
			]
		{
			const auto now = std::chrono::steady_clock::now();
			const float elapsed_time = std::chrono::duration<float>(now - _last_time).count();
			if (delta > 0.0f && elapsed_time >= delta) {
				update_canvas();
				_last_time = now;
			}
			const auto canvas = draw_canvas();
			
			return ftxui::vbox({
					ftxui::filler(),
					ftxui::hbox({
						ftxui::vbox({
							ftxui::text(title)
								| ftxui::center
								| ftxui::color(ftxui::Color::Cyan)
								| ftxui::yflex,
							ftxui::separator(),
							ftxui::hbox({
								ftxui::vbox(info_table_labels)
									| ftxui::xflex,
								ftxui::separator(),
								info_table_valuesComponent->Render() | ftxui::xflex,
							})
							| ftxui::xflex,
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
