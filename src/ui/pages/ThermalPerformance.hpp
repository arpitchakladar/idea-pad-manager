#pragma once

#include <memory>

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
  float m_CurrentAngle = 0.0F;
};
} // namespace ipm::ui::pages
