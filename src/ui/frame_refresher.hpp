#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_FRAME_REFRESHER_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_FRAME_REFRESHER_HPP

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <ftxui/component/screen_interactive.hpp>

#pragma once

namespace idea_pad_manager::ui {
class FrameRefresher {
public:
  FrameRefresher(ftxui::ScreenInteractive &Screen);
  void run();
  void setFramesPerSecond(int FramesPerSecond);
  void stop();

private:
  std::mutex m_LockMutex;
  std::condition_variable m_ConditionVariable;
  std::atomic<bool> m_Running;
  std::atomic<int> m_FramesPerSecond;
  std::thread m_Thread;
  ftxui::ScreenInteractive &m_Screen;
};
} // namespace idea_pad_manager::ui

#endif
