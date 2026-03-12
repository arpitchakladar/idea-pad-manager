#pragma once

#include <memory>
#include <string>

#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
class PowerInformation : public Page {
public:
  PowerInformation();
  static auto create() -> std::shared_ptr<PowerInformation> {
    return std::make_shared<PowerInformation>();
  }

private:
  bool m_ConservationMode;
  std::string m_ConservationModeButtonMessage;
  float m_CurrentAngle;
};
} // namespace ipm::ui::pages
