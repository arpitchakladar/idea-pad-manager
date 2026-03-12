#include "ui/pages/AboutSystem.hpp"

#include <utility>

#include <ftxui/dom/canvas.hpp>

#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
AboutSystem::AboutSystem() {
  static constexpr auto k_CanvasDimentions = std::make_pair(100, 100);
  createPage(
    { RowStatic{ "Device Model", "LOQ" },
      RowStatic{ "Serial Number", "SOMETHING" },
      RowStatic{ "Processor", "Intel Core i9-14900HX" },
      RowStatic{ "Memory", "32 GB DDR5 @ 5600MT/s" },
      RowStatic{ "GPU (Dedicated)", "RTX 5060 Laptop GPU" },
      RowStatic{ "GPU (Integrated)", "AMD Radeon" },
      RowStatic{ "Storage", "789GB / 1000GB" } },
    "About System",
    0,
    []() -> void {},
    []() -> ftxui::Canvas {
      const auto Canvas =
        ftxui::Canvas(k_CanvasDimentions.first, k_CanvasDimentions.second);
      return Canvas;
    });
}
} // namespace ipm::ui::pages
