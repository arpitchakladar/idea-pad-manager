#include "ui/pages/ThermalPerformance.hpp"

#include <ftxui/dom/canvas.hpp>

#include "sys/ThermalPerformance.hpp"
#include "ui/animations/CoolingFan.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
ThermalPerformance::ThermalPerformance() {
  static constexpr auto k_CanvasSize = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  m_CoolingFanAnimation.resize(k_CanvasSize);
  createPage(
    sys::ThermalPerformance::thermalPerformanceInfo(),
    "THERMAL & PERFORMANCE",
    m_CoolingFanAnimation.canvasUpdatesPerSecond(),
    [&]() { m_CoolingFanAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_CoolingFanAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
