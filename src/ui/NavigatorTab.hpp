#pragma once

#include <ftxui/component/component_base.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ipm::ui {
class NavigatorTab {
public:
  NavigatorTab(std::vector<std::string> Labels);
  NavigatorTab(const NavigatorTab &) = default;
  NavigatorTab &operator=(const NavigatorTab &) = default;
  NavigatorTab(NavigatorTab &&) = default;
  NavigatorTab &operator=(NavigatorTab &&) = default;
  ~NavigatorTab() = default;

  [[nodiscard]] auto component() const -> ftxui::Component { return m_Tabs; }
  [[nodiscard]] auto tabNumber() const -> uint { return m_TabNumber; }
  static auto create(std::vector<std::string> Labels)
    -> std::shared_ptr<NavigatorTab> {
    return std::make_shared<NavigatorTab>(std::move(Labels));
  }

private:
  uint m_TabNumber = 0;
  std::vector<std::string> m_Labels;
  ftxui::Component m_Tabs;
};
} // namespace ipm::ui
