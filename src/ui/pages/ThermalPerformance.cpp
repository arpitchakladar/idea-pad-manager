#include "ui/pages/ThermalPerformance.hpp"

#include <ftxui/dom/canvas.hpp>

#include "sys/ThermalPerformance.hpp"
#include "ui/animations/CoolingFan.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
ThermalPerformance::ThermalPerformance() {
  m_CoolingFanAnimation.resize(k_CanvasSize);
  createPage(
    sys::ThermalPerformance::rows(),
    "THERMAL & PERFORMANCE",
    m_CoolingFanAnimation.canvasUpdatesPerSecond(),
    [&]() { m_CoolingFanAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_CoolingFanAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
