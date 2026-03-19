#include "ui/pages/PowerInformation.hpp"

#include <optional>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include "sys/PowerInformation.hpp"
#include "ui/animations/Battery.hpp"
#include "ui/utils/CustomCanvas.hpp"
#include "ui/utils/LabelButton.hpp"

namespace ipm::ui::pages {

PowerInformation::PowerInformation() {
  auto ConservationModeButton = utils::LabelButton::create("OFF",
                                  [&]() -> std::string {
                                    m_ConservationMode = !m_ConservationMode;
                                    return m_ConservationMode ? "ON " : "OFF";
                                  }) |
    ftxui::center;

  m_BatteryAnimation.resize(k_CanvasSize);

  createPage(
    sys::PowerInformation::rows(),
    "POWER INFORMATION",
    m_BatteryAnimation.canvasUpdatesPerSecond(),
    [&]() { m_BatteryAnimation.update(); },
    [&]() -> ftxui::Canvas { return m_BatteryAnimation.drawCanvas(); });
}
} // namespace ipm::ui::pages
