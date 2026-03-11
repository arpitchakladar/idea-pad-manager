#ifndef HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_PAGES_PAGE_HPP
#define HOME_ARPIT_PROJECTS_IDEA_PAD_MANAGER_SRC_UI_PAGES_PAGE_HPP

#include <chrono>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <functional>
#include <string>
#include <tuple>

namespace ipm::ui::pages {
using RowStatic = std::tuple<std::string, std::string>;
using RowDynamic = std::tuple<std::string, std::function<std::string()>>;
using RowCustom = std::tuple<std::string, ftxui::Component>;

class Page {
public:
  auto component() -> ftxui::Component { return m_PageComponent; }
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> int {
    return m_CanvasUpdatesPerSecond;
  }

protected:
  Page() = default;
  void createPage(
      std::initializer_list<std::variant<RowStatic, RowDynamic, RowCustom>>
          Rows,
      std::string Title, int CanvasUpdatesPerSecond,
      const std::function<void()> &UpdateCanvas,
      const std::function<ftxui::Canvas()> &DrawCanvas);

private:
  ftxui::Component m_PageComponent;
  int m_CanvasUpdatesPerSecond;
  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};
} // namespace ipm::ui::pages

#endif
