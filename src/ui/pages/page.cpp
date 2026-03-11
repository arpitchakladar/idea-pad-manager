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
		int canvasUpdatesPerSecond,
		std::function<void()> updateCanvas,
		std::function<ftxui::Canvas()> drawCanvas
	) {
		_canvasUpdatesPerSecond = canvasUpdatesPerSecond;
		_lastTime = std::chrono::steady_clock::now();
		
		const float delta = canvasUpdatesPerSecond > 0.0f
			? 1.0f / static_cast<float>(canvasUpdatesPerSecond)
			: -1.0;
		std::vector<ftxui::Element> infoTableLabels;
		infoTableLabels.reserve(rows.size() * 2 - 1);
		std::vector<ftxui::Component> infoTableValues;
		infoTableValues.reserve(rows.size() * 2 - 1);
		
		auto last_ptr = rows.end() - 1;
		for (auto it = rows.begin(); it != rows.end(); it++) {
			std::visit(
				[&](auto&& row_data) {
					using T = std::decay_t<decltype(row_data)>;
					
					infoTableLabels.push_back(
						ftxui::text(std::move(std::get<0>(row_data)))
							| ftxui::color(ftxui::Color::Yellow)
							| ftxui::vcenter
					);
					
					if constexpr (std::is_same_v<
						T,
						RowCustom
					>) {
						infoTableValues.push_back(
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
						infoTableValues.push_back(
							ftxui::Renderer([row_data = std::move(row_data)] {
								return ftxui::text(get<1>(row_data)());
							})
						);
					} else if constexpr (std::is_same_v<
						T,
						RowStatic
					>) {
						infoTableValues.push_back(
							ftxui::Renderer([row_data = std::move(row_data)] {
								return ftxui::text(get<1>(row_data));
							})
						);
					}
				},
				std::move(*it)
			);
			
			if (it != last_ptr) {
				infoTableLabels.push_back(ftxui::separator());
				infoTableValues.push_back(
					ftxui::Renderer(
						[] () { return ftxui::separator(); }
					)
				);
			}
		}
		
		auto infoTableValuesComponent = ftxui::Container::Vertical(infoTableValues);
		
		_pageComponent = ftxui::Renderer(
			infoTableValuesComponent,
			[&, infoTableValuesComponent, infoTableLabels, updateCanvas, drawCanvas, delta, title = std::move(title)]
		{
			const auto now = std::chrono::steady_clock::now();
			const float elapsed_time = std::chrono::duration<float>(now - _lastTime).count();
			if (delta > 0.0f && elapsed_time >= delta) {
				updateCanvas();
				_lastTime = now;
			}
			const auto canvas = drawCanvas();
			
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
								ftxui::vbox(infoTableLabels)
									| ftxui::xflex,
								ftxui::separator(),
								infoTableValuesComponent->Render() | ftxui::xflex,
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
