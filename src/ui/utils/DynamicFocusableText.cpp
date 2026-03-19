#include "ui/utils/DynamicFocusableText.hpp"

#include <chrono>

#include "ui/utils/FocusableText.hpp"

namespace ipm::ui::utils {

DynamicFocusableText::DynamicFocusableText(
  std::function<std::optional<std::string>()> TextGetter)
  : FocusableText(TextGetter()),
    m_TextGetter(std::move(TextGetter)),
    m_PreviousReloadTime(std::chrono::steady_clock::now()) {}

auto DynamicFocusableText::OnRender() -> ftxui::Element {
  auto Now = std::chrono::steady_clock::now();
  if (Now - m_PreviousReloadTime >=
    std::chrono::seconds(k_SecondBeforeReload)) {
    m_PreviousReloadTime = Now;
    setText(m_TextGetter());
  }
  return FocusableText::OnRender();
}
} // namespace ipm::ui::utils
