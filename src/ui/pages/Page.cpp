#include "ui/pages/Page.hpp"

#include <chrono>
#include <functional>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui::pages {
auto Page::createPage(
  std::initializer_list<std::variant<RowStatic, RowDynamic, RowCustom>> Rows,
  std::string Title,
  int CanvasUpdatesPerSecond,
  const std::function<void()> &UpdateCanvas,
  const std::function<ftxui::Canvas()> &DrawCanvas) -> void {
  m_CanvasUpdatesPerSecond = CanvasUpdatesPerSecond;
  m_LastTime = std::chrono::steady_clock::now();

  const auto Delta = CanvasUpdatesPerSecond > 0
    ? 1.0F / static_cast<float>(CanvasUpdatesPerSecond)
    : -1.0F;
  std::vector<ftxui::Element> InfoTableLabels;
  InfoTableLabels.reserve((Rows.size() * 2) - 1);
  std::vector<ftxui::Component> InfoTableValues;
  InfoTableValues.reserve((Rows.size() * 2) - 1);

  const auto *LastPtr = Rows.end() - 1;
  for (const auto *It = Rows.begin(), *End = Rows.end(); It != End; ++It) {
    std::visit(
      [&](auto &&RowData) -> auto {
        using T = std::decay_t<decltype(RowData)>;

        InfoTableLabels.push_back(ftxui::text(std::move(std::get<0>(RowData))) |
          ftxui::color(ftxui::Color::Yellow) | ftxui::vcenter);

        if constexpr (std::is_same_v<T, RowCustom>) {
          const auto CustomComponent = get<1>(RowData);
          InfoTableValues.push_back(ftxui::Renderer(CustomComponent,
            [RowData = std::forward<decltype(RowData)>(RowData)]()
              -> ftxui::Element { return get<1>(RowData)->Render(); }));
        } else if constexpr (std::is_same_v<T, RowDynamic>) {
          InfoTableValues.push_back(ftxui::Renderer(
            [RowData = std::forward<decltype(RowData)>(RowData)]()
              -> ftxui::Element { return ftxui::text(get<1>(RowData)()); }));
        } else if constexpr (std::is_same_v<T, RowStatic>) {
          InfoTableValues.push_back(ftxui::Renderer(
            [RowData = std::forward<decltype(RowData)>(RowData)]()
              -> ftxui::Element { return ftxui::text(get<1>(RowData)); }));
        }
      },
      *It);

    if (It != LastPtr) {
      InfoTableLabels.push_back(ftxui::separator());
      InfoTableValues.push_back(
        ftxui::Renderer([]() -> ftxui::Element { return ftxui::separator(); }));
    }
  }

  const auto InfoTableValuesComponent =
    ftxui::Container::Vertical(InfoTableValues);

  static constexpr auto k_CanvasDimentions = std::make_pair(100, 100);

  m_PageComponent = ftxui::Renderer(InfoTableValuesComponent,
    [&,
      InfoTableValuesComponent,
      InfoTableLabels,
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

      return ftxui::vbox({
               ftxui::filler(),
               ftxui::hbox({
                 ftxui::vbox({
                   ftxui::text(Title) | ftxui::center |
                     ftxui::color(ftxui::Color::Cyan) | ftxui::yflex,

                   ftxui::separator(),

                   ftxui::hbox({
                     ftxui::vbox(InfoTableLabels) | ftxui::xflex,

                     ftxui::separator(),

                     InfoTableValuesComponent->Render() | ftxui::xflex,
                   }) |
                     ftxui::xflex,
                 }) |
                   ftxui::xflex,

                 ftxui::separator(),

                 ftxui::canvas(Canvas) | ftxui::center,
               }) |
                 ftxui::borderRounded |
                 ftxui::size(
                   ftxui::WIDTH, ftxui::EQUAL, k_CanvasDimentions.first) |
                 ftxui::center,

               ftxui::filler(),
             }) |
        ftxui::yflex;
    });
}
} // namespace ipm::ui::pages
