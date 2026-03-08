#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#pragma once

namespace idea_pad_manager::ui {
	class FrameRefresher {
	public:
		FrameRefresher(ftxui::ScreenInteractive& screen);
		~FrameRefresher();
		void run();
		void setFramesPerSecond(int framesPerSecond);
		void stop();
	
	private:
		std::mutex _lockMutex;
		std::condition_variable _conditionVariable;
		std::atomic<bool> _running;
		std::atomic<int> _framesPerSecond;
		std::thread _thread;
		ftxui::ScreenInteractive& _screen;
	};
}
