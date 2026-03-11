#include <atomic>
#include <chrono>
#include <thread>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <spdlog/spdlog.h>

#include "ui/frame_refresher.hpp"

namespace idea_pad_manager::ui {
	FrameRefresher::FrameRefresher(ftxui::ScreenInteractive& screen)
		: _screen(screen)
	{}
	
	void FrameRefresher::run() {
		if (_running) return;
		_running = true;
		_frames_per_second = 20;
		_thread = std::thread([&] {
			while (_running) {
				if (_frames_per_second <= 0) {
					std::unique_lock<std::mutex> lock(_lock_mutex);
					SPDLOG_INFO("Locked frame refresher thread");
					_condition_variable.wait(
						lock,
						[&] {
							return !_running || _frames_per_second > 0;
						}
					);
					SPDLOG_INFO("Unlocked frame refresher thread");
				} else {
					std::this_thread::sleep_for(
						std::chrono::milliseconds(1000 / _frames_per_second)
					);
					_screen.PostEvent(ftxui::Event::Custom);
				}
			}
		});
	}
	
	void FrameRefresher::set_frames_per_second(int frames_per_second) {
		if (frames_per_second != _frames_per_second) {
			SPDLOG_INFO("Changed frame refresh rate to {}", frames_per_second);
			{
				std::lock_guard<std::mutex> lock(_lock_mutex);
				_frames_per_second = frames_per_second;
			}
			_condition_variable.notify_one();
		}
	}
	
	void FrameRefresher::stop() {
		_running = false;
		_condition_variable.notify_all();
		if (_thread.joinable())
			_thread.join();
	}
	
	FrameRefresher::~FrameRefresher() {
		stop();
	}
}
