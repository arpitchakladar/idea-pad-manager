#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/utils/FocusableText.hpp"

namespace ipm::ui::utils {
class DynamicFocusableText : public FocusableText {
public:
  DynamicFocusableText(std::function<std::optional<std::string>()> TextGetter);
  auto OnRender() -> ftxui::Element override;

  static auto create(std::function<std::optional<std::string>()> Text)
    -> std::shared_ptr<DynamicFocusableText> {
    return ftxui::Make<DynamicFocusableText>(std::move(Text));
  }

private:
  std::function<std::optional<std::string>()> m_TextGetter;
  std::chrono::time_point<std::chrono::steady_clock> m_PreviousReloadTime;

  static constexpr auto k_SecondBeforeReload = 1U;
};
} // namespace ipm::ui::utils
