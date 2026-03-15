#pragma once

#include <memory>
#include <string>

#include "ui/animations/BatteryAnimation.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
class PowerInformation : public Page {
public:
  PowerInformation();
  PowerInformation(const PowerInformation &) = default;
  PowerInformation &operator=(const PowerInformation &) = default;
  PowerInformation(PowerInformation &&) = default;
  PowerInformation &operator=(PowerInformation &&) = default;
  ~PowerInformation() override = default;

  static auto create() -> std::shared_ptr<PowerInformation> {
    return std::make_shared<PowerInformation>();
  }

private:
  bool m_ConservationMode = false;
  std::string m_ConservationModeButtonMessage = "OFF";
  animations::BatteryAnimation m_BatteryAnimation;
};
} // namespace ipm::ui::pages
