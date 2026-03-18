#pragma once

#include <variant>
#include <vector>

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

  static auto thermalPerformanceInfo()
    -> std::vector<std::variant<ui::pages::RowStatic,
      ui::pages::RowDynamic,
      ui::pages::RowCustom,
      ui::pages::RowStaticError>>;
};
} // namespace ipm::sys
