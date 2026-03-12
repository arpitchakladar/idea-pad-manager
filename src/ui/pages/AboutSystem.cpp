#include "ui/pages/AboutSystem.hpp"

#include <ftxui/dom/canvas.hpp>

#include "ui/pages/Page.hpp"

namespace ipm::ui::pages {
AboutSystem::AboutSystem() {
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
      auto Canvas = ftxui::Canvas(100, 100);
      return Canvas;
    });
}
} // namespace ipm::ui::pages
