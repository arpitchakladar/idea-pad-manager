#include "ui/utils/LabelButton.hpp"
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>
#include <utility>

namespace ipm::ui::utils {

LabelButton::LabelButton(std::string Label, ClickHandler OnClick)
  : m_Label(std::move(Label)),
    m_OnClick(std::move(OnClick)) {}

auto LabelButton::OnRender() -> ftxui::Element {
  auto Inner = ftxui::text("< " + m_Label + " >");

  if (Focused()) {
    Inner = Inner | ftxui::bold | ftxui::inverted | ftxui::focus;
  } else if (Active()) {
    Inner = Inner | ftxui::select;
  }

  return Inner | ftxui::frame | ftxui::reflect(m_Box);
}

auto LabelButton::OnEvent(ftxui::Event Event) -> bool {
  if (Focused()) {
    if (Event == ftxui::Event::Return ||
      Event == ftxui::Event::Character(' ')) {
      if (m_OnClick) {
        m_Label = m_OnClick();
      }
      return true;
    }
  }

  if (Event.is_mouse() && Event.mouse().button == ftxui::Mouse::Left &&
    Event.mouse().motion == ftxui::Mouse::Pressed &&
    m_Box.Contain(Event.mouse().x, Event.mouse().y)) {
    TakeFocus();
    if (m_OnClick) {
      m_Label = m_OnClick();
    }
    return true;
  }

  return false;
}

} // namespace ipm::ui::utils
