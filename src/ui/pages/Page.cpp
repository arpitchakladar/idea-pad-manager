#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"

#include <chrono>
#include <cstddef>
#include <ftxui/dom/node.hpp>
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
namespace {

auto makeText(std::string Text) -> ftxui::Component {
  class Impl : public ftxui::ComponentBase {
  public:
    Impl(std::string Text)
      : m_Text(std::move(Text)) {}
    ftxui::Element OnRender() override {
      auto Element = ftxui::text(m_Text);

      if (Focused()) {
        Element = ftxui::focus(Element);
      } else if (Active()) {
        Element = ftxui::select(Element);
      }

      return Element;
    }

    [[nodiscard]] auto Focusable() const -> bool final { return true; }

  private:
    std::string m_Text;
  };

  return ftxui::Make<Impl>(std::move(Text));
}
} // namespace

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
  std::vector<ftxui::Component> InfoTableRows;
  InfoTableRows.reserve((Rows.size() * 2) - 1);

  auto MaxLabelLength = std::size_t(0);
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
  MaxLabelLength = MaxLabelLength + 1;

  auto I = std::size_t(0);
  const auto RowsSize = Rows.size();

  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = std::size_t(100),
    .Height = std::size_t(100),
  };
  for (auto &&Row : Rows) {
    const auto RowComponent = std::visit(
      [&](auto &&RowData) -> auto {
        using T = std::decay_t<decltype(RowData)>;

        auto LabelText = std::string();
        const auto &LabelTextContent = std::get<0>(RowData);
        static constexpr auto k_LeftPadding = std::size_t(1);
        static constexpr auto k_RightPadding = std::size_t(1);
        LabelText.reserve(MaxLabelLength + k_LeftPadding + k_RightPadding);
        const auto Padding = MaxLabelLength - LabelTextContent.size();
        for (auto I = std::size_t(0), E = Padding + k_LeftPadding; I < E; I++) {
          LabelText.push_back(' ');
        }
        for (const auto &Char : LabelTextContent) {
          LabelText.push_back(Char);
        }
        for (auto I = std::size_t(0); I < k_RightPadding; I++) {
          LabelText.push_back(' ');
        }

        const auto InfoTableRowLabel = ftxui::text(std::move(LabelText)) |
          ftxui::color(ftxui::Color::Yellow) | ftxui::vcenter;

        auto InfoTableRowValue = ftxui::Component();
        if constexpr (std::is_same_v<T, RowCustom>) {
          const auto CustomComponent = get<1>(RowData);
          InfoTableRowValue = ftxui::Renderer(CustomComponent,
            [RowData = std::forward<decltype(RowData)>(RowData)]()
              -> ftxui::Element { return get<1>(RowData)->Render(); });
        } else if constexpr (std::is_same_v<T, RowDynamic>) {
          const auto Text =
            static_cast<ftxui::Component>(makeText(get<1>(RowData)()));
          InfoTableRowValue = ftxui::Renderer(Text,
            [RowData = std::forward<decltype(RowData)>(RowData),
              Text]() -> ftxui::Element { return Text->Render(); });
        } else if constexpr (std::is_same_v<T, RowStatic>) {
          const auto Text =
            static_cast<ftxui::Component>(makeText(get<1>(RowData)));
          InfoTableRowValue = ftxui::Renderer(Text,
            [RowData = std::forward<decltype(RowData)>(RowData),
              Text]() -> ftxui::Element { return Text->Render(); });
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

      static constexpr auto k_MaxPageHeight = 20;

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
