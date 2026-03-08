#include <chrono>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

#include "ui/pages/page.hpp"

namespace idea_pad_manager::ui {
	void Page::CreatePage(
		ftxui::Component infoTable,
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
		_pageComponent = ftxui::Renderer(
			infoTable,
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
							infoTable->Render(),
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
