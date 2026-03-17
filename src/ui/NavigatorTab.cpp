#include "ui/NavigatorTab.hpp"

#include <bit>
#include <string>
#include <utility>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui {
NavigatorTab::NavigatorTab(std::vector<std::string> Labels)
  : m_Labels(std::move(Labels)) {

  const auto Tabs =
    ftxui::Toggle(&m_Labels, std::bit_cast<int *>(&m_TabNumber));

  m_Tabs = ftxui::Renderer(Tabs, [Tabs]() -> ftxui::Element {
    return Tabs->Render() | ftxui::bgcolor(ftxui::Color::Black) | ftxui::center;
  });
}
} // namespace ipm::ui
