#pragma once

#include "ui/pages/Page.hpp"

#include <memory>

namespace ipm::ui::pages {
class AboutSystem : public Page {
public:
  AboutSystem();
  ~AboutSystem() override = default;

  static auto create() -> std::shared_ptr<AboutSystem> {
    return std::make_shared<AboutSystem>();
  }
};
} // namespace ipm::ui::pages
