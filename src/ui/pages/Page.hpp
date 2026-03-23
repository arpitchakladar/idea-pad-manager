#pragma once

#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>

#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
using StaticText = std::optional<std::string>;
using DynamicText = std::function<std::optional<std::string>()>;

struct RowButton {
  std::string DefaultLabel;
  std::optional<std::function<std::string()>> OnClick;
};

struct RowDropdown {
  std::vector<std::string> Options;
  uint InitialIndex;
  std::optional<std::function<void(uint, const std::string)>> OnSelect;
};

struct Row {
  std::string Label;
  std::variant<StaticText, DynamicText, RowButton, RowDropdown> Value;
};
using Rows = std::vector<Row>;

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
  void createPage(Rows Rows,
    std::string Title,
    uint CanvasUpdatesPerSecond,
    const std::function<void()> &UpdateCanvas,
    const std::function<ftxui::Canvas()> &DrawCanvas);

  static constexpr auto k_CanvasSize = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };

private:
  ftxui::Component m_PageComponent;
  uint m_CanvasUpdatesPerSecond = 0;
  std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
};
} // namespace ipm::ui::pages
