#pragma once

#include <memory>

#include "ui/animations/CoolingFan.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
class ThermalPerformance : public Page {
public:
  ThermalPerformance();
  ThermalPerformance(const ThermalPerformance &) = default;
  ThermalPerformance &operator=(const ThermalPerformance &) = default;
  ThermalPerformance(ThermalPerformance &&) = default;
  ThermalPerformance &operator=(ThermalPerformance &&) = default;
  ~ThermalPerformance() override = default;

  static auto create() -> std::shared_ptr<ThermalPerformance> {
    return std::make_shared<ThermalPerformance>();
  }

private:
  animations::CoolingFan m_CoolingFanAnimation;
};

} // namespace ipm::ui::pages
