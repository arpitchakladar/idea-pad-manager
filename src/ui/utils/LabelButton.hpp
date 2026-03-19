#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <functional>
#include <memory>
#include <string>

namespace ipm::ui::utils {

class LabelButton : public ftxui::ComponentBase {
public:
  using ClickHandler = std::function<std::string()>;

  explicit LabelButton(std::string Label, ClickHandler OnClick);
  LabelButton(const LabelButton &) = delete;
  LabelButton &operator=(const LabelButton &) = delete;
  LabelButton(LabelButton &&) = delete;
  LabelButton &operator=(LabelButton &&) = delete;
  ~LabelButton() override = default;

  auto OnRender() -> ftxui::Element override;
  [[nodiscard]] auto OnEvent(ftxui::Event Event) -> bool override;
  [[nodiscard]] auto Focusable() const -> bool final { return true; }

  auto setLabel(std::string Label) -> void { m_Label = std::move(Label); }
  [[nodiscard]] auto getLabel() const -> const std::string & { return m_Label; }

  static auto create(std::string Label, ClickHandler OnClick)
    -> std::shared_ptr<LabelButton> {
    return ftxui::Make<LabelButton>(std::move(Label), std::move(OnClick));
  }

private:
  std::string m_Label;
  ClickHandler m_OnClick;
  ftxui::Box m_Box;
};

} // namespace ipm::ui::utils
