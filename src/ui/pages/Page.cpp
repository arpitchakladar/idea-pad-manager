#include "ui/pages/Page.hpp"

#include <chrono>
#include <functional>
#include <string>
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
#include "ui/utils/FocusableText.hpp"

namespace ipm::ui::pages {

auto Page::createPage(
  std::vector<std::variant<RowStatic, RowDynamic, RowCustom>> Rows,
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

  auto MaxLabelLength = 0U;
  for (const auto &Row : Rows) {
    std::visit(
      [&MaxLabelLength](auto &&RowData) -> void {
        const auto &Label = std::get<0>(RowData);
        if (Label.length() > MaxLabelLength) {
          MaxLabelLength = Label.length();
        }
      },
      Row);
  }
  MaxLabelLength = MaxLabelLength + 2U;

  auto I = 0U;
  const auto RowsSize = Rows.size();

  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  for (auto &&Row : Rows) {
    const auto RowComponent = std::visit<ftxui::Component>(
      [&](auto &&RowData) -> auto {
        using T = std::decay_t<decltype(RowData)>;

        auto LabelText = std::string(std::move(std::get<0>(RowData)));
        LabelText.push_back(' ');

        const auto InfoTableRowLabel = ftxui::text(std::move(LabelText)) |
          ftxui::align_right | ftxui::color(ftxui::Color::Yellow) |
          ftxui::vcenter |
          ftxui::size(
            ftxui::WIDTH, ftxui::EQUAL, static_cast<int>(MaxLabelLength));

        auto InfoTableRowValue = ftxui::Component();
        if constexpr (std::is_same_v<T, RowCustom>) {
          const auto CustomComponent = std::get<1>(RowData);
          InfoTableRowValue = ftxui::Renderer(CustomComponent,
            [RowData = std::forward<decltype(RowData)>(RowData)]()
              -> ftxui::Element { return std::get<1>(RowData)->Render(); });
        } else if constexpr (std::is_same_v<T, RowDynamic>) {
          InfoTableRowValue =
            utils::FocusableText::create(std::get<1>(RowData)());
        } else if constexpr (std::is_same_v<T, RowStatic>) {
          InfoTableRowValue =
            utils::FocusableText::create(std::get<1>(RowData));
        }

        return ftxui::Renderer(InfoTableRowValue,
          [InfoTableRowLabel, InfoTableRowValue]() -> ftxui::Element {
            return ftxui::hbox({ InfoTableRowLabel,
              ftxui::separator(),
              InfoTableRowValue->Render() });
          });
      },
      Row);

    InfoTableRows.push_back(RowComponent);
    if (++I < RowsSize) {
      InfoTableRows.push_back(
        ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }));
    }
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

      return ftxui::vbox({
               ftxui::filler(),
               ftxui::hbox({
                 ftxui::vbox({
                   ftxui::text(Title) | ftxui::center |
                     ftxui::color(ftxui::Color::Cyan) | ftxui::yflex,

                   ftxui::separator(),

                   InfoTableRowsComponent->Render() | ftxui::vscroll_indicator |
                     ftxui::frame |
                     ftxui::size(
                       ftxui::HEIGHT, ftxui::LESS_THAN, k_MaxPageHeight),
                 }) |
                   ftxui::xflex,

                 ftxui::separator(),

                 ftxui::canvas(Canvas) | ftxui::center,
               }) |
                 ftxui::borderHeavy | ftxui::clear_under |
                 ftxui::size(
                   ftxui::WIDTH, ftxui::EQUAL, k_CanvasDimentions.Width) |
                 ftxui::center,

               ftxui::filler(),
             }) |
        ftxui::yflex;
    });
}
} // namespace ipm::ui::pages
