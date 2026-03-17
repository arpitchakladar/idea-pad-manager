#include "ui/utils/FocusableText.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui::utils {
namespace {
auto base64Encode(std::string const &Input) -> std::string {
  static constexpr std::string_view k_Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  static constexpr auto k_BitsPerChar = 6;
  static constexpr auto k_BitsPerByte = 8;
  static constexpr auto k_Base64Block = 4;
  static constexpr auto k_InitialValB = -k_BitsPerChar;
  static constexpr auto k_CharMask = 0x3F; // 6 bits set

  std::string Output;
  auto Val = 0;
  auto ValB = k_InitialValB;
  for (unsigned char C : Input) {
    Val = (Val << k_BitsPerByte) + C;
    ValB += k_BitsPerByte;
    while (ValB >= 0) {
      Output.push_back(k_Chars[(Val >> ValB) & k_CharMask]);
      ValB -= k_BitsPerChar;
    }
  }
  if (ValB > k_InitialValB) {
    Output.push_back(
      k_Chars[((Val << k_BitsPerByte) >> (ValB + k_BitsPerByte)) & k_CharMask]);
  }
  while (Output.size() % static_cast<std::size_t>(k_Base64Block) != 0UZ) {
    Output.push_back('=');
  }
  return Output;
}
} // namespace

FocusableText::FocusableText(std::string Text)
  : m_Text(std::move(Text)) {}

auto FocusableText::OnRender() -> ftxui::Element {
  auto Element = ftxui::text(m_Text.substr(m_Offset));

  if (Focused()) {
    Element = ftxui::focus(Element) | ftxui::bold | ftxui::underlined;
  } else if (Active()) {
    Element = ftxui::select(Element);
  }

  return Element | ftxui::frame | ftxui::reflect(m_Box);
}

auto FocusableText::OnEvent(ftxui::Event Event) -> bool {
  if (Focused()) {
    if (Event == ftxui::Event::ArrowRight) {
      if (std::cmp_less(m_Offset, static_cast<int>(m_Text.size()) - 1)) {
        auto BoxWidth = m_Box.x_max - m_Box.x_min;
        auto MaxOffset = static_cast<int>(m_Text.size()) - BoxWidth;
        if (std::cmp_less(m_Offset, MaxOffset)) {
          m_Offset++;
        }
      }
      return true;
    }
    if (Event == ftxui::Event::ArrowLeft) {
      if (m_Offset > 0) {
        m_Offset--;
      }
      return true;
    }
    if (Event == ftxui::Event::Return) {
      auto const Encoded = base64Encode(m_Text);
      auto const Sequence = "\033]52;c;" + Encoded + "\a";
      std::cout << Sequence << std::flush;
      return true;
      return true;
    }
  }
  if (Event.is_mouse() && Event.mouse().button == ftxui::Mouse::Left &&
    Event.mouse().motion == ftxui::Mouse::Pressed &&
    m_Box.Contain(Event.mouse().x, Event.mouse().y)) {
    TakeFocus();
    return true;
  }
  return false;
}
} // namespace ipm::ui::utils
