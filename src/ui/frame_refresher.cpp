#include <atomic>
#include <chrono>
#include <thread>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <spdlog/spdlog.h>

#include "ui/frame_refresher.hpp"

namespace UI {
	FrameRefresher::FrameRefresher(ftxui::ScreenInteractive& screen)
		: _screen(screen)
	{}
	
	void FrameRefresher::run() {
		if (_running) return;
		_running = true;
		_framesPerSecond = 20;
		_thread = std::thread([&] {
			while (_running) {
				if (_framesPerSecond <= 0) {
					std::unique_lock<std::mutex> lock(_lockMutex);
					SPDLOG_INFO("Locked frame refresher thread");
					_conditionVariable.wait(
						lock,
						[&] {
							return !_running || _framesPerSecond > 0;
						}
					);
					SPDLOG_INFO("Unlocked frame refresher thread");
				} else {
					std::this_thread::sleep_for(
						std::chrono::milliseconds(1000 / _framesPerSecond)
					);
					_screen.PostEvent(ftxui::Event::Custom);
				}
			}
		});
	}
	
	void FrameRefresher::setFramesPerSecond(int framesPerSecond) {
		if (framesPerSecond != _framesPerSecond) {
			SPDLOG_INFO("Changed frame refresh rate to {}", framesPerSecond);
			{
				std::lock_guard<std::mutex> lock(_lockMutex);
				_framesPerSecond = framesPerSecond;
			}
			_conditionVariable.notify_one();
		}
	}
	
	void FrameRefresher::stop() {
		_running = false;
		_conditionVariable.notify_all();
		if (_thread.joinable())
			_thread.join();
	}
	
	FrameRefresher::~FrameRefresher() {
		stop();
	}
}
