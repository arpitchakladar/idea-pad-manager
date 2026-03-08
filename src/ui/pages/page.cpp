#include <chrono>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

#include "ui/pages/page.hpp"

#pragma once

namespace UI {
	void Page::CreatePage(
		ftxui::Component infoTable,
		std::string title,
		int fps,
		std::function<ftxui::Canvas()> drawFrame
	) {
		_lastTime = std::chrono::steady_clock::now();
		
		_canvas = drawFrame();
		const float delta = fps > 0
			? 1.0f / static_cast<float>(fps)
			: -1.0;
		_pageComponent = ftxui::Renderer(
			infoTable,
			[&, infoTable, drawFrame, delta, title = std::move(title)]
		{
			const auto now = std::chrono::steady_clock::now();
			const float elapsed_time = (now - _lastTime).count() / 1e9f;
			if (delta > 0 && elapsed_time >= delta) {
				_lastTime = now;
				_canvas = drawFrame();
			}

			return ftxui::vbox({
					ftxui::filler(),
					ftxui::hbox({
						ftxui::vbox({
							ftxui::text(std::to_string(elapsed_time))
								| ftxui::center
								| ftxui::color(ftxui::Color::Cyan)
								| ftxui::yflex,
							ftxui::separator(),
							infoTable->Render(),
						})
							| ftxui::xflex,
						ftxui::separator(),
						ftxui::canvas(_canvas) | ftxui::center,
					})
						| ftxui::borderRounded
						| ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 100)
						| ftxui::center,
					ftxui::filler(),
			}) | ftxui::yflex;
		});
	}
}
