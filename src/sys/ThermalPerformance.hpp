#pragma once

#include "ui/pages/Page.hpp"

namespace ipm::sys {
class ThermalPerformance {
public:
  ThermalPerformance() = delete;
  ThermalPerformance(const ThermalPerformance &) = delete;
  ThermalPerformance &operator=(const ThermalPerformance &) = delete;
  ThermalPerformance(ThermalPerformance &&) = delete;
  ThermalPerformance &operator=(ThermalPerformance &&) = delete;
  ~ThermalPerformance() = delete;

  static auto thermalPerformanceInfo() -> ui::pages::Rows;
};
} // namespace ipm::sys
