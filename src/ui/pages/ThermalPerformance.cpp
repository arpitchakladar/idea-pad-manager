#include "ui/pages/ThermalPerformance.hpp"

#include <ftxui/dom/canvas.hpp>

#include "ui/pages/Page.hpp"
#include "ui/utils/CustomCanvas.hpp"

namespace ipm::ui::pages {
ThermalPerformance::ThermalPerformance() {
  static constexpr auto k_CanvasDimentions = utils::CanvasSize{
    .Width = 100U,
    .Height = 100U,
  };
  createPage(
    { RowDynamic{ "CPU Temperature", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 1", []() -> const char * { return "64°C"; } },
      RowDynamic{ "CPU Core 2", []() -> const char * { return "66°C"; } },
      RowDynamic{ "CPU Core 3", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 4", []() -> const char * { return "67°C"; } },
      RowDynamic{ "CPU Core 5", []() -> const char * { return "64°C"; } },
      RowDynamic{ "CPU Core 6", []() -> const char * { return "65°C"; } },
      RowDynamic{ "CPU Core 7", []() -> const char * { return "66°C"; } },
      RowDynamic{ "CPU Core 8", []() -> const char * { return "68°C"; } },
      RowDynamic{ "GPU Temperature", []() -> const char * { return "58°C"; } },
      RowStatic{ "Thermal Mode", "Balanced" },
      RowStatic{ "CPU Frequency", "4.2 GHz" },
      RowStatic{ "CPU Core 1", "4.1 GHz" },
      RowStatic{ "CPU Core 2", "4.3 GHz" },
      RowStatic{ "CPU Core 3", "4.2 GHz" },
      RowStatic{ "CPU Core 4", "4.1 GHz" },
      RowStatic{ "CPU Core 5", "4.2 GHz" },
      RowStatic{ "CPU Core 6", "4.3 GHz" },
      RowStatic{ "CPU Core 7", "4.2 GHz" },
      RowStatic{ "CPU Core 8", "4.1 GHz" },
      RowStatic{ "CPU Governor", "performance" },
      RowStatic{ "Min Frequency", "800 MHz" },
      RowStatic{ "Max Frequency", "5.0 GHz" } },
    "Thermal & Performance",
    0,
    []() -> void {},
    []() -> ftxui::Canvas { return utils::CustomCanvas(k_CanvasDimentions); });
}
} // namespace ipm::ui::pages
