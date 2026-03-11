#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_NAVIGATOR_TAB_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_NAVIGATOR_TAB_HPP

#include <memory>
#include <vector>

#include <ftxui/component/component.hpp>

#pragma once

namespace idea_pad_manager::ui {
class NavigatorTab {
public:
  NavigatorTab(std::vector<std::string> Labels);
  auto component() -> ftxui::Component { return m_Tabs; }
  [[nodiscard]] auto tabNumber() const -> int { return m_TabNumber; }
  static auto create(std::vector<std::string> Labels)
      -> std::shared_ptr<NavigatorTab> {
    return std::make_shared<NavigatorTab>(std::move(Labels));
  }

private:
  int m_TabNumber;
  std::vector<std::string> m_Labels;
  ftxui::Component m_Tabs;
};
} // namespace idea_pad_manager::ui

#endif
