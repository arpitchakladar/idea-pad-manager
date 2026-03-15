#include "ui/pages/AboutSystem.hpp"

#include <ftxui/dom/canvas.hpp>

#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
AboutSystem::AboutSystem() {
  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
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
    []() -> ftxui::Canvas { return utils::CustomCanvas(k_CanvasDimentions); });
}
} // namespace ipm::ui::pages
