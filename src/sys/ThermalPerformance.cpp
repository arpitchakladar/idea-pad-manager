#include "sys/ThermalPerformance.hpp"

#include <format>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::sys {

namespace {

constexpr auto k_MillidegToDegDivisor = 1000.0F;

auto readTemperatureValue(std::string_view Path) -> std::optional<std::string> {
  auto File = utils::File(std::string(Path));
  if (!File.isRegular()) {
    return "N/A";
  }

  auto Raw = File.read();
  if (!Raw.has_value()) {
    return "N/A";
  }

  long Millideg = 0;
  try {
    Millideg = std::stol(Raw.value());
  } catch (...) {
    return "N/A";
  }

  return std::format("{:.1f}\xC2\xB0"
                     "C",
    static_cast<float>(Millideg) / k_MillidegToDegDivisor);
}

auto getDeviceName(std::string_view HwmonPath, std::string_view HwmonName)
  -> std::string {
  auto NameFile = utils::File(std::string(HwmonPath) + "/name");
  if (NameFile.isRegular()) {
    auto NameRaw = NameFile.read();
    if (NameRaw.has_value() && !NameRaw->empty()) {
      return NameRaw.value();
    }
  }
  return std::string(HwmonName);
}

auto getTemperatureLabel(std::string_view HwmonPath,
  std::string_view Filename,
  std::string_view DeviceName) -> std::string {
  auto LabelFilename = std::string(Filename);
  LabelFilename.replace(
    LabelFilename.find("_input"), std::string_view("_label").size(), "_label");
  auto LabelFile = utils::File(std::string(HwmonPath) + "/" + LabelFilename);

  if (LabelFile.isRegular()) {
    auto LabelRaw = LabelFile.read();
    if (LabelRaw.has_value() && !LabelRaw->empty()) {
      return std::string(DeviceName) + " \xe2\x80\x94 " + LabelRaw.value();
    }
  }
  return std::string(DeviceName);
}

auto createTemperatureUpdater(std::string InputPath)
  -> std::function<std::optional<std::string>()> {
  return [Path = std::move(InputPath)]() -> std::optional<std::string> {
    return readTemperatureValue(Path);
  };
}

void processTemperatureInput(ui::pages::Rows &Rows,
  std::string_view HwmonPath,
  std::string_view Filename,
  std::string_view DeviceName) {
  auto InputPath = std::string(HwmonPath) + "/" + std::string(Filename);

  auto Label = getTemperatureLabel(HwmonPath, Filename, DeviceName);

  auto TempFile = utils::File(InputPath);
  if (!TempFile.isRegular()) {
    Rows.emplace_back(std::make_tuple(std::move(Label), std::nullopt));
    return;
  }

  auto Updater = createTemperatureUpdater(std::move(InputPath));
  Rows.emplace_back(std::make_tuple(std::move(Label), std::move(Updater)));
}

void processHwmonDevice(ui::pages::Rows &Rows, std::string_view HwmonName) {

  constexpr auto k_Base = "/sys/class/hwmon";
  auto HwmonPath = std::string(k_Base) + "/" + std::string(HwmonName);

  auto DeviceName = getDeviceName(HwmonPath, HwmonName);

  auto HwmonDir = utils::Directory(HwmonPath);
  if (!HwmonDir.isOpen()) {
    return;
  }

  HwmonDir.forEachChild([&](std::string_view Filename) -> void {
    if (!Filename.ends_with("_input") || !Filename.starts_with("temp")) {
      return;
    }
    processTemperatureInput(Rows, HwmonPath, Filename, DeviceName);
  });
}

} // namespace

auto ThermalPerformance::thermalPerformanceInfo() -> ui::pages::Rows {

  auto Rows = ui::pages::Rows();

  constexpr auto k_Base = "/sys/class/hwmon";

  auto BaseDir = utils::Directory(k_Base);
  if (!BaseDir.isOpen()) {
    return Rows;
  }

  BaseDir.forEachChild([&Rows](std::string_view HwmonName) -> void {
    if (!HwmonName.starts_with("hwmon")) {
      return;
    }
    processHwmonDevice(Rows, HwmonName);
  });

  return Rows;
}

} // namespace ipm::sys
