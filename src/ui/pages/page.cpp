#include <chrono>
#include <ftxui/dom/node.hpp>
#include <initializer_list>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/pages/page.hpp"

namespace idea_pad_manager::ui::pages {
	void Page::CreatePage(
		std::initializer_list<
			std::pair<
				std::string,
				std::pair<
					std::function<std::string()>,
					ftxui::Component
				>
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
		for (auto it = rows.begin(); it != rows.end(); ++it) {
			auto&& row = *it;
			infoTableLabels.push_back(ftxui::text(row.first)
				| ftxui::color(ftxui::Color::Yellow));
			infoTableValues.push_back(
				row.second.second == nullptr
					? ftxui::Renderer(
						[getText = row.second.first] () {
							return ftxui::text(getText());
						}
					)
					: ftxui::Renderer(
						row.second.second,
						[getText = row.second.first, component = row.second.second] () {
							return ftxui::hbox(
								ftxui::text(getText()),
								component
							);
						}
					)
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
		
		auto infoTable = ftxui::hbox({
				ftxui::vbox(infoTableLabels)
					| ftxui::xflex,
				ftxui::separator(),
				infoTableValuesComponent->Render() | ftxui::xflex,
			})
			| ftxui::xflex;
		
		_pageComponent = ftxui::Renderer(
			infoTableValuesComponent,
			[&, infoTable, updateCanvas, drawCanvas, delta, title = std::move(title)]
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
							infoTable,
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
