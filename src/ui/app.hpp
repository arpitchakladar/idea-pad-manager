#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>

#pragma once

namespace UI {
	class App {
	public:
		App();
		~App();
		void setup();
		void run();
	
	private:
		std::mutex _frameRefresherLockMutex;
		std::condition_variable _frameRefresherConditionVariable;
		std::atomic<bool> _running;
		std::atomic<int> _framesPerSecond;
		std::thread _frameRefresher;
		ftxui::Component _app;
		ftxui::ScreenInteractive _screen;
	};
}
