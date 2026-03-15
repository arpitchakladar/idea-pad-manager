#pragma once

#include <memory>

#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
class AboutSystem : public Page {
public:
  AboutSystem();
  AboutSystem(const AboutSystem &) = default;
  AboutSystem &operator=(const AboutSystem &) = default;
  AboutSystem(AboutSystem &&) = default;
  AboutSystem &operator=(AboutSystem &&) = default;
  ~AboutSystem() override = default;

  static auto create() -> std::shared_ptr<AboutSystem> {
    return std::make_shared<AboutSystem>();
  }
};
} // namespace ipm::ui::pages
