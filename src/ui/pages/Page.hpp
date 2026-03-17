#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <variant>

namespace ipm::ui::pages {
using RowStatic = std::tuple<std::string, std::string>;
using RowDynamic = std::tuple<std::string, std::function<std::string()>>;
using RowCustom = std::tuple<std::string, ftxui::Component>;
using RowStaticError = std::tuple<std::string>;

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
    std::vector<std::variant<RowStatic, RowDynamic, RowCustom, RowStaticError>>
      Rows,
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
