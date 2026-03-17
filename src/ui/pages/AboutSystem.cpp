#include "ui/pages/AboutSystem.hpp"

#include <ftxui/dom/canvas.hpp>

#include "sys/AboutSystem.hpp"
#include "ui/animations/HostName.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
AboutSystem::AboutSystem() {
  static constexpr auto k_CanvasSize = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  m_HostNameAnimation.resize(k_CanvasSize);
  createPage(
    sys::AboutSystem::aboutSystemInfo(),
    "ABOUT SYSTEM",
    m_HostNameAnimation.canvasUpdatesPerSecond(),
    [&]() { m_HostNameAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_HostNameAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
