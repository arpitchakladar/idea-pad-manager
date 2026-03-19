#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>

namespace ipm::ui::utils {

class LabelButton : public ftxui::ComponentBase {
public:
  using ClickHandler = std::function<std::string()>;

  explicit LabelButton(std::string Label, std::optional<ClickHandler> OnClick);
  LabelButton(const LabelButton &) = delete;
  LabelButton &operator=(const LabelButton &) = delete;
  LabelButton(LabelButton &&) = delete;
  LabelButton &operator=(LabelButton &&) = delete;
  ~LabelButton() override = default;

  auto OnRender() -> ftxui::Element override;
  [[nodiscard]] auto OnEvent(ftxui::Event Event) -> bool override;
  [[nodiscard]] auto Focusable() const -> bool final {
    return m_OnClick.has_value();
  }

  auto setLabel(std::string Label) -> void { m_Label = std::move(Label); }
  [[nodiscard]] auto getLabel() const -> const std::string & { return m_Label; }

  static auto create(std::string Label, std::optional<ClickHandler> OnClick)
    -> std::shared_ptr<LabelButton> {
    return ftxui::Make<LabelButton>(std::move(Label), std::move(OnClick));
  }

private:
  std::string m_Label;
  std::optional<ClickHandler> m_OnClick;
  ftxui::Box m_Box;

  static constexpr auto k_NullOptDisplayText = "N/A";
  static constexpr auto k_NullOptDisplayColor = ftxui::Color::Red;
};

} // namespace ipm::ui::utils
