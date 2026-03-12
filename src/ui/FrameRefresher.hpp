#pragma once

#include <atomic>
#include <condition_variable>
#include <ftxui/component/screen_interactive.hpp>
#include <mutex>
#include <thread>

namespace ipm::ui {
class FrameRefresher {
public:
  FrameRefresher(ftxui::ScreenInteractive& Screen);
  auto run() -> void;
  auto setFramesPerSecond(int FramesPerSecond) -> void;
  auto stop() -> void;

private:
  std::mutex m_LockMutex;
  std::condition_variable m_ConditionVariable;
  std::atomic<bool> m_Running;
  std::atomic<int> m_FramesPerSecond;
  std::thread m_Thread;
  ftxui::ScreenInteractive& m_Screen;
};
} // namespace ipm::ui
