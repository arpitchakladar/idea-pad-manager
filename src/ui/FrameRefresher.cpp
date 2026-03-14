#include "ui/FrameRefresher.hpp"

#include <atomic>
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <mutex>
#include <spdlog/spdlog.h>
#include <thread>

namespace ipm::ui {
FrameRefresher::FrameRefresher(ftxui::ScreenInteractive &Screen)
  : m_Screen(Screen) {}

FrameRefresher::~FrameRefresher() { stop(); }

auto FrameRefresher::run() -> void {
  if (m_Running) {
    return;
  }
  m_Running = true;
  static constexpr auto k_DefaultFramesPerSecond = 20;
  m_FramesPerSecond = k_DefaultFramesPerSecond;
  m_Thread = std::thread([&]() -> void {
    while (m_Running) {
      if (m_FramesPerSecond <= 0) {
        auto Lock = std::unique_lock(m_LockMutex);
        SPDLOG_INFO("Locked frame refresher thread");
        m_ConditionVariable.wait(
          Lock, [&]() -> bool { return !m_Running || m_FramesPerSecond > 0; });
        SPDLOG_INFO("Unlocked frame refresher thread");
      } else {
        static constexpr auto k_MillisecondsInSecond = 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(
          k_MillisecondsInSecond / m_FramesPerSecond));
        m_Screen.PostEvent(ftxui::Event::Custom);
      }
    }
  });
}

auto FrameRefresher::setFramesPerSecond(int FramesPerSecond) -> void {
  if (FramesPerSecond == m_FramesPerSecond) {
    return;
  }
  SPDLOG_INFO("Changed frame refresh rate to {}", FramesPerSecond);
  {
    const auto Lock = std::scoped_lock(m_LockMutex);
    m_FramesPerSecond = FramesPerSecond;
  }
  m_ConditionVariable.notify_one();
}

auto FrameRefresher::stop() -> void {
  m_Running = false;
  m_ConditionVariable.notify_all();
  if (m_Thread.joinable()) {
    m_Thread.join();
  }
}
} // namespace ipm::ui
