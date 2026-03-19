#pragma once

#include <ftxui/screen/box.hpp>
#include <ftxui/screen/color.hpp>
#include <memory>
#include <optional>
#include <string>
#include <sys/types.h>
#include <utility>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui::utils {
class FocusableText : public ftxui::ComponentBase {
public:
  FocusableText(std::optional<std::string> Text);
  FocusableText(const FocusableText &) = delete;
  FocusableText &operator=(const FocusableText &) = delete;
  FocusableText(FocusableText &&) = delete;
  FocusableText &operator=(FocusableText &&) = delete;
  ~FocusableText() override = default;

  auto OnRender() -> ftxui::Element override;
  [[nodiscard]] auto OnEvent(ftxui::Event Event) -> bool override;

  [[nodiscard]] auto Focusable() const -> bool final {
    return m_Text.has_value();
  }

  auto setText(std::optional<std::string> Text) -> void {
    m_Text = std::move(Text);
  }

  static auto create(std::optional<std::string> Text)
    -> std::shared_ptr<FocusableText> {
    return ftxui::Make<FocusableText>(std::move(Text));
  }

private:
  std::optional<std::string> m_Text;
  ftxui::Box m_Box;
  uint m_Offset = 0U;

  static constexpr auto k_NullOptDisplayText = "N/A";
  static constexpr auto k_NullOptDisplayColor = ftxui::Color::Red;
};
} // namespace ipm::ui::utils
