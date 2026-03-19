#include "sys/PowerInformation.hpp"
#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"
#include <array>
#include <format>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace ipm::sys {

namespace {

constexpr auto k_PowerSupplyBase = "/sys/class/power_supply";
constexpr auto k_MicroToUnit = 1'000'000.0F;
constexpr auto k_MicroToMilli = 1'000.0F;
constexpr auto k_TenthDegToDegDivisor = 10.0F;
constexpr auto k_MinutesPerHour = 60L;

auto readRaw(std::string_view Path) -> std::optional<std::string> {
  auto File = utils::File(std::string(Path));
  if (!File.isRegular()) {
    return std::nullopt;
  }
  return File.read();
}

auto readLong(std::string_view Path) -> std::optional<long> {
  auto Raw = readRaw(Path);
  if (!Raw.has_value() || Raw->empty()) {
    return std::nullopt;
  }
  try {
    return std::stol(Raw.value());
  } catch (...) {
    return std::nullopt;
  }
}

auto readPercent(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format("{}%", Val.value());
}

auto readMicroVoltAsVolt(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format(
    "{:.3f} V", static_cast<float>(Val.value()) / k_MicroToUnit);
}

auto readMicroAmpAsAmp(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format(
    "{:.3f} A", static_cast<float>(Val.value()) / k_MicroToUnit);
}

auto readMicroWattAsWatt(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format(
    "{:.2f} W", static_cast<float>(Val.value()) / k_MicroToUnit);
}

auto readMicroWattHourAsMilliWattHour(std::string_view Path)
  -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format(
    "{:.1f} mWh", static_cast<float>(Val.value()) / k_MicroToMilli);
}

auto readMicroAmpHourAsMilliAmpHour(std::string_view Path)
  -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format(
    "{:.1f} mAh", static_cast<float>(Val.value()) / k_MicroToMilli);
}

auto readCycleCount(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format("{} cycles", Val.value());
}

auto readMinutesAsTime(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  auto Minutes = Val.value();
  if (Minutes <= 0) {
    return std::nullopt;
  }
  auto Hours = Minutes / k_MinutesPerHour;
  auto Mins = Minutes % k_MinutesPerHour;
  return std::format("{}h {:02d}m", Hours, Mins);
}

auto readTemperatureTenth(std::string_view Path) -> std::optional<std::string> {
  auto Val = readLong(Path);
  if (!Val.has_value()) {
    return std::nullopt;
  }
  return std::format("{:.1f}\xC2\xB0"
                     "C",
    static_cast<float>(Val.value()) / k_TenthDegToDegDivisor);
}

auto readHealthString(std::string_view Path) -> std::optional<std::string> {
  return readRaw(Path);
}

auto readStatusString(std::string_view Path) -> std::optional<std::string> {
  return readRaw(Path);
}

using ReaderFn = std::function<std::optional<std::string>(std::string_view)>;

auto makeUpdater(std::string FilePath, const ReaderFn &Reader)
  -> std::function<std::optional<std::string>()> {
  return [Path = std::move(FilePath), Reader]() -> std::optional<std::string> {
    return Reader(Path);
  };
}

struct Attribute {
  std::string_view Filename;
  std::string_view Label;
  ReaderFn Reader;
  bool LiveUpdate;
};

const std::array<Attribute, 30> g_Attributes = { {
  // Status / health
  { .Filename = "status",
    .Label = "Status",
    .Reader = readStatusString,
    .LiveUpdate = false },
  { .Filename = "health",
    .Label = "Health",
    .Reader = readHealthString,
    .LiveUpdate = false },
  { .Filename = "present",
    .Label = "Present",
    .Reader = readPercent,
    .LiveUpdate = false },

  // Charge level
  { .Filename = "capacity",
    .Label = "Charge",
    .Reader = readPercent,
    .LiveUpdate = true },
  { .Filename = "capacity_level",
    .Label = "Charge Level",
    .Reader = readStatusString,
    .LiveUpdate = true },

  // Voltage
  { .Filename = "voltage_now",
    .Label = "Voltage (now)",
    .Reader = readMicroVoltAsVolt,
    .LiveUpdate = true },
  { .Filename = "voltage_min_design",
    .Label = "Voltage (min)",
    .Reader = readMicroVoltAsVolt,
    .LiveUpdate = false },
  { .Filename = "voltage_max_design",
    .Label = "Voltage (max)",
    .Reader = readMicroVoltAsVolt,
    .LiveUpdate = false },
  { .Filename = "voltage_avg",
    .Label = "Voltage (avg)",
    .Reader = readMicroVoltAsVolt,
    .LiveUpdate = true },

  // Current / power
  { .Filename = "current_now",
    .Label = "Current (now)",
    .Reader = readMicroAmpAsAmp,
    .LiveUpdate = true },
  { .Filename = "current_avg",
    .Label = "Current (avg)",
    .Reader = readMicroAmpAsAmp,
    .LiveUpdate = true },
  { .Filename = "power_now",
    .Label = "Power (now)",
    .Reader = readMicroWattAsWatt,
    .LiveUpdate = true },

  // Energy (µWh-based drivers, e.g. ACPI)
  { .Filename = "energy_now",
    .Label = "Energy (now)",
    .Reader = readMicroWattHourAsMilliWattHour,
    .LiveUpdate = true },
  { .Filename = "energy_full",
    .Label = "Energy (full)",
    .Reader = readMicroWattHourAsMilliWattHour,
    .LiveUpdate = true },
  { .Filename = "energy_full_design",
    .Label = "Energy (design)",
    .Reader = readMicroWattHourAsMilliWattHour,
    .LiveUpdate = false },
  { .Filename = "energy_empty",
    .Label = "Energy (empty)",
    .Reader = readMicroWattHourAsMilliWattHour,
    .LiveUpdate = false },

  // Charge (µAh-based drivers, e.g. some embedded / mobile)
  { .Filename = "charge_now",
    .Label = "Charge (now)",
    .Reader = readMicroAmpHourAsMilliAmpHour,
    .LiveUpdate = true },
  { .Filename = "charge_full",
    .Label = "Charge (full)",
    .Reader = readMicroAmpHourAsMilliAmpHour,
    .LiveUpdate = true },
  { .Filename = "charge_full_design",
    .Label = "Charge (design)",
    .Reader = readMicroAmpHourAsMilliAmpHour,
    .LiveUpdate = false },
  { .Filename = "charge_empty",
    .Label = "Charge (empty)",
    .Reader = readMicroAmpHourAsMilliAmpHour,
    .LiveUpdate = false },
  { .Filename = "charge_counter",
    .Label = "Charge Counter",
    .Reader = readMicroAmpHourAsMilliAmpHour,
    .LiveUpdate = true },

  // Longevity
  { .Filename = "cycle_count",
    .Label = "Cycle Count",
    .Reader = readCycleCount,
    .LiveUpdate = false },

  // Time estimates
  { .Filename = "time_to_empty_now",
    .Label = "Time to Empty",
    .Reader = readMinutesAsTime,
    .LiveUpdate = true },
  { .Filename = "time_to_full_now",
    .Label = "Time to Full",
    .Reader = readMinutesAsTime,
    .LiveUpdate = true },

  // Temperature
  { .Filename = "temp",
    .Label = "Temperature",
    .Reader = readTemperatureTenth,
    .LiveUpdate = true },

  // Manufacture info (read-once strings)
  { .Filename = "manufacturer",
    .Label = "Manufacturer",
    .Reader = readStatusString,
    .LiveUpdate = false },
  { .Filename = "model_name",
    .Label = "Model",
    .Reader = readStatusString,
    .LiveUpdate = false },
  { .Filename = "serial_number",
    .Label = "Serial Number",
    .Reader = readStatusString,
    .LiveUpdate = false },
  { .Filename = "technology",
    .Label = "Technology",
    .Reader = readStatusString,
    .LiveUpdate = false },
  { .Filename = "type",
    .Label = "Type",
    .Reader = readStatusString,
    .LiveUpdate = false },
} };

void processPowerSupplyDevice(
  ui::pages::Rows &Rows, std::string_view DeviceName) {
  auto DevicePath =
    std::string(k_PowerSupplyBase) + "/" + std::string(DeviceName);

  auto TypeFile = utils::File(DevicePath + "/type");
  if (!TypeFile.isRegular()) {
    return;
  }
  auto TypeRaw = TypeFile.read();
  if (!TypeRaw.has_value()) {
    return;
  }

  Rows.emplace_back(ui::pages::Row{
    .Label = std::string(DeviceName),
    .Value = std::nullopt,
  });

  for (const auto &Attr : g_Attributes) {
    auto FilePath = DevicePath + "/" + std::string(Attr.Filename);
    auto File = utils::File(FilePath);
    if (!File.isRegular()) {
      continue;
    }

    auto Label = std::string(Attr.Label);

    if (Attr.LiveUpdate) {
      auto Updater = makeUpdater(std::move(FilePath), Attr.Reader);
      Rows.emplace_back(ui::pages::Row{
        .Label = std::move(Label),
        .Value = std::move(Updater),
      });
    } else {
      auto StaticValue = Attr.Reader(FilePath);
      if (!StaticValue.has_value()) {
        continue;
      }
      auto Captured = std::move(StaticValue);
      Rows.emplace_back(ui::pages::Row{
        .Label = std::move(Label),
        .Value = [Val = std::move(Captured)]() -> std::optional<std::string> {
          return Val;
        },
      });
    }
  }
}

} // namespace

auto PowerInformation::rows() -> ui::pages::Rows {
  auto Rows = ui::pages::Rows();

  auto BaseDir = utils::Directory(k_PowerSupplyBase);
  if (!BaseDir.isOpen()) {
    return Rows;
  }

  BaseDir.forEachEntry([&Rows](std::string_view DeviceName) -> void {
    processPowerSupplyDevice(Rows, DeviceName);
  });

  return Rows;
}

} // namespace ipm::sys
