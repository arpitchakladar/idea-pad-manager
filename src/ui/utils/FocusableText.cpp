#include "ui/utils/FocusableText.hpp"

#include <ftxui/dom/elements.hpp>
#include <string>
#include <utility>

namespace ipm::ui::utils {
FocusableText::FocusableText(std::string Text)
  : m_Text(std::move(Text)) {}

auto FocusableText::OnRender() -> ftxui::Element {
  auto Element = ftxui::text(m_Text);

  if (Focused()) {
    Element = ftxui::focus(Element);
  } else if (Active()) {
    Element = ftxui::select(Element);
  }

  return Element;
}
} // namespace ipm::ui::utils
