#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include "ui/navigator_tab.hpp"

namespace idea_pad_manager::ui {
NavigatorTab::NavigatorTab(std::vector<std::string> Labels) {
  m_TabNumber = 0;
  m_Labels = std::move(Labels);

  auto Tabs = ftxui::Toggle(&m_Labels, &m_TabNumber);

  m_Tabs = ftxui::Renderer(Tabs, [Tabs] {
    return Tabs->Render() | ftxui::bgcolor(ftxui::Color::Black) | ftxui::center;
  });
}
} // namespace idea_pad_manager::ui
