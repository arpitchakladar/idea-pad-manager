#pragma once

#include <ftxui/screen/box.hpp>
#include <memory>
#include <string>
#include <utility>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui::utils {
class FocusableText : public ftxui::ComponentBase {
public:
  FocusableText(std::string Text);
  auto OnRender() -> ftxui::Element override;
  [[nodiscard]] auto OnEvent(ftxui::Event Event) -> bool override;

  [[nodiscard]] auto Focusable() const -> bool final { return true; }

  auto setText(std::string Text) -> void { m_Text = std::move(Text); }

  static auto create(std::string Text) -> std::shared_ptr<FocusableText> {
    return ftxui::Make<FocusableText>(std::move(Text));
  }

private:
  std::string m_Text;
  ftxui::Box m_Box;
  uint m_Offset = 0U;
};
} // namespace ipm::ui::utils
