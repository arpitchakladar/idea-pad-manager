#include "ui/pages/Page.hpp"

#include <chrono>
#include <ftxui/screen/color.hpp>
#include <functional>
#include <memory>
#include <string>
#include <sys/types.h>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

#include "ui/utils/CustomCanvas.hpp"
#include "ui/utils/DynamicFocusableText.hpp"
#include "ui/utils/FocusableText.hpp"

namespace ipm::ui::pages {

auto Page::createPage(Rows Rows,
  std::string Title,
  uint CanvasUpdatesPerSecond,
  const std::function<void()> &UpdateCanvas,
  const std::function<ftxui::Canvas()> &DrawCanvas) -> void {
  m_CanvasUpdatesPerSecond = CanvasUpdatesPerSecond;
  m_LastTime = std::chrono::steady_clock::now();

  const auto Delta = CanvasUpdatesPerSecond > 0
    ? 1.0F / static_cast<float>(CanvasUpdatesPerSecond)
    : -1.0F;
  std::vector<ftxui::Component> InfoTableRows;
  InfoTableRows.reserve((Rows.size() * 2UZ) - 1UZ);

  static constexpr auto k_ContentDimensions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };

  static constexpr auto k_LabelLength =
    ((k_ContentDimensions.Width / utils::CanvasSize::k_CharacterWidth) / 2U) -
    2U;

  static constexpr auto k_RemainingSpace =
    (k_ContentDimensions.Width / 2U) - k_LabelLength - 5U;

  ;
  for (auto &&Row : std::move(Rows)) {
    auto LabelText = std::string(std::move(Row.Label));
    LabelText.push_back(' ');

    auto RowLabelElement = ftxui::text(std::move(LabelText)) |
      ftxui::align_right | ftxui::color(ftxui::Color::Yellow) | ftxui::vcenter |
      ftxui::size(ftxui::WIDTH, ftxui::EQUAL, static_cast<int>(k_LabelLength));

    auto RowValueComponent = std::visit<ftxui::Component>(
      [&]<typename T>(T RowValue) -> ftxui::Component {
        if constexpr (std::is_same_v<T, CustomComponent>) {
          return ftxui::Renderer(RowValue,
            [RowValueComponent = std::move(RowValue)]() -> ftxui::Element {
              return RowValueComponent->Render();
            });
        } else if constexpr (std::is_same_v<T, DynamicText>) {
          const auto InfoTableRowValueTextComponent =
            utils::DynamicFocusableText::create(std::move(RowValue));
          return ftxui::Renderer(InfoTableRowValueTextComponent,
            [InfoTableRowValueTextComponent =
                std::move(InfoTableRowValueTextComponent)]() -> ftxui::Element {
              return InfoTableRowValueTextComponent->Render();
            });
        } else if constexpr (std::is_same_v<T, StaticText>) {
          return utils::FocusableText::create(std::move(RowValue));
        }
      },
      std::move(Row.Value));

    auto RowComponent = ftxui::Renderer(RowValueComponent,
      [RowLabelElement = std::move(RowLabelElement),
        RowValueComponent = std::move(RowValueComponent)]() -> ftxui::Element {
        return ftxui::hbox({ RowLabelElement,
          ftxui::separator(),
          RowValueComponent->Render() |
            ftxui::size(ftxui::WIDTH,
              ftxui::EQUAL,
              static_cast<int>(k_RemainingSpace)) });
      });

    InfoTableRows.emplace_back(std::move(RowComponent));
    InfoTableRows.emplace_back(
      ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }));
  }

  const auto InfoTableRowsComponent =
    ftxui::Container::Vertical(std::move(InfoTableRows));

  m_PageComponent = ftxui::Renderer(InfoTableRowsComponent,
    [&,
      InfoTableRowsComponent,
      UpdateCanvas,
      DrawCanvas,
      Delta,
      Title = std::move(Title)]() -> ftxui::Element {
      const auto Now = std::chrono::steady_clock::now();
      const auto ElapsedTime =
        std::chrono::duration<float>(Now - m_LastTime).count();
      if (Delta > 0.0F && ElapsedTime >= Delta) {
        UpdateCanvas();
        m_LastTime = Now;
      }
      const auto Canvas = DrawCanvas();

      static constexpr auto k_MaxPageHeight = 20U;
      static constexpr auto k_TitleHeight = 5U;

      return ftxui::vbox({
               ftxui::filler(),
               ftxui::hbox({
                 ftxui::vbox({
                   ftxui::text(Title) | ftxui::center |
                     ftxui::color(ftxui::Color::Cyan) |
                     ftxui::size(ftxui::HEIGHT,
                       ftxui::EQUAL,
                       static_cast<int>(k_TitleHeight)),

                   ftxui::separator(),

                   InfoTableRowsComponent->Render() | ftxui::vscroll_indicator |
                     ftxui::frame |
                     ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, k_MaxPageHeight),
                 }) |
                   ftxui::xflex,

                 ftxui::separator(),

                 ftxui::canvas(Canvas) | ftxui::center,
               }) |
                 ftxui::borderHeavy | ftxui::clear_under |
                 ftxui::size(
                   ftxui::WIDTH, ftxui::EQUAL, k_ContentDimensions.Width) |
                 ftxui::center,

               ftxui::filler(),
             }) |
        ftxui::yflex;
    });
}
} // namespace ipm::ui::pages
