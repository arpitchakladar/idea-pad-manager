#pragma once

#include "ui/pages/Page.hpp"

#include <memory>

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
};
} // namespace ipm::ui::pages
