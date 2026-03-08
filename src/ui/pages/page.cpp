#include <chrono>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/pages/page.hpp"

#pragma once

namespace UI {
	void Page::CreatePage(
		ftxui::Component infoTable,
		std::string title,
		std::function<ftxui::Canvas(float)> drawFrame
	) {
		_lastTime = std::chrono::steady_clock::now();

		_pageComponent = ftxui::Renderer(
			infoTable,
			[&, infoTable, drawFrame, title]
		{
			const auto now = std::chrono::steady_clock::now();
			const auto elapsed_time = (now - _lastTime).count();
			const ftxui::Canvas canvas = drawFrame(elapsed_time);
			_lastTime = now;

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
