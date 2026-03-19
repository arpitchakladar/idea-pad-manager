#include "sys/ThermalPerformance.hpp"

#include <format>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"

namespace ipm::sys {
namespace {
constexpr auto k_MillidegToDegDivisor = 1000.0F;
constexpr auto k_HwmonBase = "/sys/class/hwmon";

auto readTemperatureValue(std::string_view Path) -> std::optional<std::string> {
  auto File = utils::File(std::string(Path));
  if (!File.isRegular()) {
    return std::nullopt;
  }
  auto Raw = File.read();
  if (!Raw.has_value()) {
    return std::nullopt;
  }
  auto Millideg = 0L;
  try {
    Millideg = std::stol(Raw.value());
  } catch (...) {
    return std::nullopt;
  }
  return std::format("{:.1f}\xC2\xB0"
                     "C",
    static_cast<float>(Millideg) / k_MillidegToDegDivisor);
}

auto readFanValue(std::string_view Path) -> std::optional<std::string> {
  auto File = utils::File(std::string(Path));
  if (!File.isRegular()) {
    return std::nullopt;
  }
  auto Raw = File.read();
  if (!Raw.has_value()) {
    return std::nullopt;
  }
  auto Rpm = 0L;
  try {
    Rpm = std::stol(Raw.value());
  } catch (...) {
    return std::nullopt;
  }
  return std::format("{} RPM", Rpm);
}

auto readNameFromDirectory(std::string_view DirectoryPath,
  std::string_view FallbackName) -> std::string {
  auto NameFile = utils::File(std::string(DirectoryPath) + "/name");
  if (NameFile.isRegular()) {
    auto NameRaw = NameFile.read();
    if (NameRaw.has_value() && !NameRaw->empty()) {
      return NameRaw.value();
    }
  }
  return std::string(FallbackName);
}

auto readLabelFromInput(std::string_view BasePath,
  std::string_view InputFilename,
  std::string_view FallbackLabel) -> std::string {
  auto LabelFilename = std::string(InputFilename);
  LabelFilename.replace(
    LabelFilename.find("_input"), std::string_view("_label").size(), "_label");
  auto LabelFile = utils::File(std::string(BasePath) + "/" + LabelFilename);
  if (LabelFile.isRegular()) {
    auto LabelRaw = LabelFile.read();
    if (LabelRaw.has_value() && !LabelRaw->empty()) {
      return std::string(FallbackLabel) + " \xe2\x80\x94 " + LabelRaw.value();
    }
  }
  return std::string(FallbackLabel);
}

auto createFileUpdater(std::string FilePath)
  -> std::function<std::optional<std::string>()> {
  return [Path = std::move(FilePath)]() -> std::optional<std::string> {
    return readTemperatureValue(Path);
  };
}

void processSensorInput(ui::pages::Rows &Rows,
  std::string_view BasePath,
  std::string_view Filename,
  std::string_view DeviceName) {
  auto InputPath = std::string(BasePath) + "/" + std::string(Filename);
  auto Label = readLabelFromInput(BasePath, Filename, DeviceName);
  auto TempFile = utils::File(InputPath);
  if (!TempFile.isRegular()) {
    Rows.emplace_back(
      ui::pages::Row{ .Label = std::move(Label), .Value = std::nullopt });
    return;
  }
  auto Updater = createFileUpdater(std::move(InputPath));
  Rows.emplace_back(
    ui::pages::Row{ .Label = std::move(Label), .Value = std::move(Updater) });
}

void processFanInput(ui::pages::Rows &Rows,
  std::string_view BasePath,
  std::string_view Filename,
  std::string_view DeviceName) {
  auto InputPath = std::string(BasePath) + "/" + std::string(Filename);
  auto Label = std::string(DeviceName) + " Fan";
  auto FanFile = utils::File(InputPath);
  if (!FanFile.isRegular()) {
    return;
  }
  auto Updater = [Path = std::move(InputPath)]() -> std::optional<std::string> {
    return readFanValue(Path);
  };
  Rows.emplace_back(
    ui::pages::Row{ .Label = std::move(Label), .Value = std::move(Updater) });
}

void processHwmonDevice(ui::pages::Rows &Rows, std::string_view HwmonName) {
  auto HwmonPath = std::string(k_HwmonBase) + "/" + std::string(HwmonName);
  auto DeviceName = readNameFromDirectory(HwmonPath, HwmonName);
  auto HwmonDir = utils::Directory(HwmonPath);
  if (!HwmonDir.isOpen()) {
    return;
  }
  HwmonDir.forEachEntry([&](std::string_view Filename) -> void {
    if (Filename.ends_with("_input") && Filename.starts_with("temp")) {
      processSensorInput(Rows, HwmonPath, Filename, DeviceName);
    } else if (Filename.ends_with("_input") && Filename.starts_with("fan")) {
      processFanInput(Rows, HwmonPath, Filename, DeviceName);
    }
  });
}
} // namespace

auto ThermalPerformance::rows() -> ui::pages::Rows {
  auto Rows = ui::pages::Rows();
  auto BaseDir = utils::Directory(k_HwmonBase);
  if (!BaseDir.isOpen()) {
    return Rows;
  }
  BaseDir.forEachEntry([&Rows](std::string_view HwmonName) -> void {
    if (!HwmonName.starts_with("hwmon")) {
      return;
    }
    processHwmonDevice(Rows, HwmonName);
  });
  return Rows;
}
} // namespace ipm::sys
