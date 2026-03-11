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
FrameRefresher::FrameRefresher(ftxui::ScreenInteractive &Screen)
    : m_Screen(Screen) {}

void FrameRefresher::run() {
  if (m_Running) {
    return;
  }
  m_Running = true;
  m_FramesPerSecond = 20;
  m_Thread = std::thread([&] () -> void {
    while (m_Running) {
      if (m_FramesPerSecond <= 0) {
        std::unique_lock<std::mutex> Lock(m_LockMutex);
        SPDLOG_INFO("Locked frame refresher thread");
        m_ConditionVariable.wait(
            Lock, [&] () -> bool { return !m_Running || m_FramesPerSecond > 0; });
        SPDLOG_INFO("Unlocked frame refresher thread");
      } else {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(1000 / m_FramesPerSecond));
        m_Screen.PostEvent(ftxui::Event::Custom);
      }
    }
  });
}

void FrameRefresher::setFramesPerSecond(int FramesPerSecond) {
  if (FramesPerSecond != m_FramesPerSecond) {
    SPDLOG_INFO("Changed frame refresh rate to {}", FramesPerSecond);
    {
      std::lock_guard<std::mutex> lock(m_LockMutex);
      m_FramesPerSecond = FramesPerSecond;
    }
    m_ConditionVariable.notify_one();
  }
}

void FrameRefresher::stop() {
  m_Running = false;
  m_ConditionVariable.notify_all();
  if (m_Thread.joinable()) {
    m_Thread.join();
  }
}
} // namespace idea_pad_manager::ui
