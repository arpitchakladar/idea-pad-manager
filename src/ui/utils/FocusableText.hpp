#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

namespace ipm::ui::utils {
class FocusableText : public ftxui::ComponentBase {
public:
  FocusableText(std::string Text);
  auto OnRender() -> ftxui::Element override;

  [[nodiscard]] auto Focusable() const -> bool final { return true; }

  static auto create(std::string Text) -> ftxui::Component {
    return ftxui::Make<FocusableText>(std::move(Text));
  }

private:
  std::string m_Text;
};
} // namespace ipm::ui::utils
