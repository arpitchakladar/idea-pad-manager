#pragma once

#include "ui/pages/Page.hpp"

namespace ipm::sys {

class PowerInformation {
public:
  PowerInformation() = delete;
  PowerInformation(const PowerInformation &) = delete;
  PowerInformation &operator=(const PowerInformation &) = delete;
  PowerInformation(PowerInformation &&) = delete;
  PowerInformation &operator=(PowerInformation &&) = delete;
  ~PowerInformation() = delete;

  static auto rows() -> ui::pages::Rows;
};

} // namespace ipm::sys
