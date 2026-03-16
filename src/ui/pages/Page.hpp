#pragma once

#include <chrono>
#include <functional>
#include <initializer_list>
#include <string>
#include <sys/types.h>
#include <tuple>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <variant>

namespace ipm::ui::pages {
using RowStatic = std::tuple<std::string, std::string>;
using RowDynamic = std::tuple<std::string, std::function<std::string()>>;
using RowCustom = std::tuple<std::string, ftxui::Component>;

class Page {
public:
  Page() = default;
  Page(const Page &) = default;
  Page &operator=(const Page &) = default;
  Page(Page &&) = default;
  Page &operator=(Page &&) = default;
  virtual ~Page() = default;

  [[nodiscard]] auto component() const -> ftxui::Component {
    return m_PageComponent;
  }
  [[nodiscard]] auto canvasUpdatesPerSecond() const -> uint {
    return m_CanvasUpdatesPerSecond;
  }

protected:
  void createPage(
    std::initializer_list<std::variant<RowStatic, RowDynamic, RowCustom>> Rows,
    std::string Title,
    uint CanvasUpdatesPerSecond,
    const std::function<void()> &UpdateCanvas,
    const std::function<ftxui::Canvas()> &DrawCanvas);

private:
  ftxui::Component m_PageComponent;
  uint m_CanvasUpdatesPerSecond = 0;
  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};
} // namespace ipm::ui::pages
