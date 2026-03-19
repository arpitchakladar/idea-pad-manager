#include "ui/utils/LabelButton.hpp"

#include <optional>
#include <utility>

#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui::utils {

LabelButton::LabelButton(std::string Label, std::optional<ClickHandler> OnClick)
  : m_Label(std::move(Label)),
    m_OnClick(std::move(OnClick)) {}

auto LabelButton::OnRender() -> ftxui::Element {
  if (!m_OnClick.has_value()) {
    return ftxui::text(k_NullOptDisplayText) |
      ftxui::color(k_NullOptDisplayColor);
  }
  auto Inner = ftxui::text("< " + m_Label + " >");

  if (Focused()) {
    Inner = Inner | ftxui::bold | ftxui::inverted | ftxui::focus;
  } else if (Active()) {
    Inner = Inner | ftxui::select;
  }

  return Inner | ftxui::frame | ftxui::reflect(m_Box);
}

auto LabelButton::OnEvent(ftxui::Event Event) -> bool {
  if (Focused() && m_OnClick.has_value()) {
    if (Event == ftxui::Event::Return ||
      Event == ftxui::Event::Character(' ')) {
      m_Label = m_OnClick.value()();
      return true;
    }
  }

  if (Event.is_mouse() && Event.mouse().button == ftxui::Mouse::Left &&
    Event.mouse().motion == ftxui::Mouse::Pressed &&
    m_Box.Contain(Event.mouse().x, Event.mouse().y)) {
    TakeFocus();
    if (m_OnClick.has_value()) {
      m_Label = m_OnClick.value()();
    }
    return true;
  }

  return false;
}

} // namespace ipm::ui::utils
