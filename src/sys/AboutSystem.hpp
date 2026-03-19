#pragma once

#include "ui/pages/Page.hpp"

namespace ipm::sys {
class AboutSystem {
public:
  AboutSystem() = delete;
  AboutSystem(const AboutSystem &) = delete;
  AboutSystem &operator=(const AboutSystem &) = delete;
  AboutSystem(AboutSystem &&) = delete;
  AboutSystem &operator=(AboutSystem &&) = delete;
  ~AboutSystem() = delete;

  static auto aboutSystemInfo() -> ui::pages::Rows;
};
} // namespace ipm::sys
