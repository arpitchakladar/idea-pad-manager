#include "ui/NavigatorTab.hpp"

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace ipm::ui {
NavigatorTab::NavigatorTab(std::vector<std::string> Labels) {
    m_TabNumber = 0;
    m_Labels = std::move(Labels);

    auto Tabs = ftxui::Toggle(&m_Labels, &m_TabNumber);

    m_Tabs = ftxui::Renderer(Tabs, [Tabs]() -> ftxui::Element {
        return Tabs->Render() | ftxui::bgcolor(ftxui::Color::Black) |
            ftxui::center;
    });
}
} // namespace ipm::ui
