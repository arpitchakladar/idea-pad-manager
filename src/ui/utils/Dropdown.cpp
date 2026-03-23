#include "ui/utils/Dropdown.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ftxui/component/component_base.hpp>
#include <string>
#include <utility>

#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>

namespace ipm::ui::utils {

Dropdown::Dropdown(std::vector<std::string> Options,
  std::size_t SelectedIndex,
  SelectHandler OnSelect)
  : m_Options(std::move(Options)),
    m_SelectedIndex(
      std::min(SelectedIndex, m_Options.empty() ? 0UZ : m_Options.size() - 1)),
    m_HoveredIndex(m_SelectedIndex),
    m_OnSelect(std::move(OnSelect)) {
  m_ItemBoxes.resize(m_Options.size());
}

auto Dropdown::setOptions(std::vector<std::string> Options) -> void {
  m_Options = std::move(Options);
  m_ItemBoxes.resize(m_Options.size());
  m_SelectedIndex =
    std::min(m_SelectedIndex, m_Options.empty() ? 0UZ : m_Options.size() - 1);
  m_HoveredIndex = m_SelectedIndex;
  m_IsOpen = false;
}

auto Dropdown::setSelectedIndex(std::size_t Index) -> void {
  assert(Index < m_Options.size());
  m_SelectedIndex = Index;
  m_HoveredIndex = Index;
}

auto Dropdown::getSelectedValue() const -> const std::string & {
  assert(!m_Options.empty());
  return m_Options[m_SelectedIndex];
}

auto Dropdown::confirmSelection(std::size_t Index) -> void {
  m_SelectedIndex = Index;
  close();
  if (m_OnSelect) {
    m_OnSelect(m_SelectedIndex, m_Options[m_SelectedIndex]);
  }
}

auto Dropdown::OnRender() -> ftxui::Element {
  const auto Focused = ftxui::ComponentBase::Focused();
  const auto Active = ftxui::ComponentBase::Active();

  const std::string Arrow = m_IsOpen ? " ▲" : " ▼";
  const std::string HeaderTxt =
    (m_Options.empty() ? "(empty)" : m_Options[m_SelectedIndex]) + Arrow;

  auto Header =
    ftxui::text("[ " + HeaderTxt + " ]") | ftxui::color(ftxui::Color::Blue);

  if (Focused && !m_IsOpen) {
    Header = Header | ftxui::bold | ftxui::inverted | ftxui::focus;
  } else if (Active && !m_IsOpen) {
    Header = Header | ftxui::select;
  }

  Header = Header | ftxui::reflect(m_HeaderBox);

  if (!m_IsOpen) {
    return Header | ftxui::frame;
  }

  ftxui::Elements Items;
  Items.reserve(m_Options.size());

  for (std::size_t I = 0; I < m_Options.size(); ++I) {
    const bool IsSelected = (I == m_SelectedIndex);
    const bool IsHovered = (I == m_HoveredIndex);

    auto ItemText = ftxui::text("  " + m_Options[I] + "  ");

    if (IsSelected && IsHovered) {
      ItemText = ItemText | ftxui::bold | ftxui::inverted;
    } else if (IsHovered) {
      ItemText = ItemText | ftxui::inverted;
    } else if (IsSelected) {
      ItemText = ItemText | ftxui::bold | ftxui::color(ftxui::Color::Blue);
    }

    Items.push_back(ItemText | ftxui::reflect(m_ItemBoxes[I]));
  }

  auto Menu = ftxui::vbox(std::move(Items)) | ftxui::border;

  return ftxui::vbox({ Header, Menu }) | ftxui::frame;
}

auto Dropdown::OnEvent(ftxui::Event Event) -> bool {
  if (!Focused()) {
    return false;
  }

  if (!m_IsOpen) {
    return handleClosedStateEvent(Event);
  }
  return handleOpenStateEvent(Event);
}

auto Dropdown::handleClosedStateEvent(const ftxui::Event &Event) -> bool {
  if (Event == ftxui::Event::Return || Event == ftxui::Event::Character(' ')) {
    open();
    return true;
  }
  return false;
}

auto Dropdown::handleOpenStateEvent(ftxui::Event &Event) -> bool {
  if (Event.is_mouse()) {
    return handleMouseEvent(Event);
  }
  return handleKeyboardNavigation(Event);
}

auto Dropdown::handleKeyboardNavigation(const ftxui::Event &Event) -> bool {
  if (Event == ftxui::Event::Return || Event == ftxui::Event::Character(' ')) {
    confirmSelection(m_HoveredIndex);
    return true;
  }

  if (Event == ftxui::Event::Escape) {
    close();
    return true;
  }

  if (Event == ftxui::Event::ArrowUp || Event == ftxui::Event::Character('k')) {
    if (m_HoveredIndex > 0) {
      --m_HoveredIndex;
    }
    return true;
  }

  if (Event == ftxui::Event::ArrowDown ||
    Event == ftxui::Event::Character('j')) {
    if (!m_Options.empty() && m_HoveredIndex < m_Options.size() - 1) {
      ++m_HoveredIndex;
    }
    return true;
  }

  if (Event == ftxui::Event::Home) {
    m_HoveredIndex = 0;
    return true;
  }
  if (Event == ftxui::Event::End && !m_Options.empty()) {
    m_HoveredIndex = m_Options.size() - 1;
    return true;
  }

  return false;
}

auto Dropdown::handleMouseEvent(ftxui::Event &Event) -> bool {
  const auto &Mouse = Event.mouse();

  if (Mouse.button == ftxui::Mouse::Left &&
    Mouse.motion == ftxui::Mouse::Pressed) {
    return handleMouseClick(Mouse);
  }

  if (Mouse.motion == ftxui::Mouse::Moved && m_IsOpen) {
    return handleMouseMove(Mouse);
  }

  return false;
}

auto Dropdown::handleMouseClick(const ftxui::Mouse &Mouse) -> bool {
  if (m_HeaderBox.Contain(Mouse.x, Mouse.y)) {
    TakeFocus();
    m_IsOpen ? close() : open();
    return true;
  }

  if (m_IsOpen) {
    for (std::size_t I = 0; I < m_ItemBoxes.size(); ++I) {
      if (m_ItemBoxes[I].Contain(Mouse.x, Mouse.y)) {
        TakeFocus();
        confirmSelection(I);
        return true;
      }
    }
    close();
    return true;
  }

  return false;
}

auto Dropdown::handleMouseMove(const ftxui::Mouse &Mouse) -> bool {
  for (std::size_t I = 0; I < m_ItemBoxes.size(); ++I) {
    if (m_ItemBoxes[I].Contain(Mouse.x, Mouse.y)) {
      m_HoveredIndex = I;
      return true;
    }
  }
  return false;
}

} // namespace ipm::ui::utils
