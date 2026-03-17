#pragma once

#include <variant>
#include <vector>

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

  static auto aboutSystemInfo()
    -> std::vector<std::variant<ui::pages::RowStatic,
      ui::pages::RowDynamic,
      ui::pages::RowCustom,
      ui::pages::RowStaticError>>;
};
} // namespace ipm::sys
