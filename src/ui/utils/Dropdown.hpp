#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>

namespace ipm::ui::utils {

class Dropdown : public ftxui::ComponentBase {
public:
  using SelectHandler =
    std::function<void(std::size_t Index, const std::string &Value)>;

  explicit Dropdown(std::vector<std::string> Options,
    std::size_t SelectedIndex,
    SelectHandler OnSelect);

  Dropdown(const Dropdown &) = delete;
  Dropdown &operator=(const Dropdown &) = delete;
  Dropdown(Dropdown &&) = delete;
  Dropdown &operator=(Dropdown &&) = delete;
  ~Dropdown() override = default;

  auto OnRender() -> ftxui::Element override;
  [[nodiscard]] auto OnEvent(ftxui::Event Event) -> bool override;

  [[nodiscard]] auto Focusable() const -> bool final { return true; }

  auto setOptions(std::vector<std::string> Options) -> void;
  auto setSelectedIndex(std::size_t Index) -> void;
  [[nodiscard]] auto getSelectedIndex() const -> std::size_t {
    return m_SelectedIndex;
  }
  [[nodiscard]] auto getSelectedValue() const -> const std::string &;

  static auto create(std::vector<std::string> Options,
    std::size_t SelectedIndex,
    SelectHandler OnSelect) -> std::shared_ptr<Dropdown> {
    return ftxui::Make<Dropdown>(
      std::move(Options), SelectedIndex, std::move(OnSelect));
  }

private:
  auto open() -> void {
    m_IsOpen = true;
    m_HoveredIndex = m_SelectedIndex;
  }
  auto close() -> void { m_IsOpen = false; }
  auto confirmSelection(std::size_t Index) -> void;

  auto handleClosedStateEvent(const ftxui::Event &Event) -> bool;
  auto handleOpenStateEvent(ftxui::Event &Event) -> bool;
  auto handleKeyboardNavigation(const ftxui::Event &Event) -> bool;
  auto handleMouseEvent(ftxui::Event &Event) -> bool;
  auto handleMouseClick(const ftxui::Mouse &Mouse) -> bool;
  auto handleMouseMove(const ftxui::Mouse &Mouse) -> bool;

  std::vector<std::string> m_Options;
  std::size_t m_SelectedIndex{ 0 };
  std::size_t m_HoveredIndex{ 0 };
  bool m_IsOpen{ false };
  SelectHandler m_OnSelect;

  ftxui::Box m_HeaderBox;
  std::vector<ftxui::Box> m_ItemBoxes;
};

} // namespace ipm::ui::utils
