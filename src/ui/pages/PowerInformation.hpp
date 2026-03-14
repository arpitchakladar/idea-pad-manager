#pragma once

#include "ui/pages/Page.hpp"

#include <memory>
#include <string>

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
  std::string m_ConservationModeButtonMessage;
  float m_CurrentAngle = 0.0F;
};
} // namespace ipm::ui::pages
