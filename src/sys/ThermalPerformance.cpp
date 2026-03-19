#include "sys/ThermalPerformance.hpp"
#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"
#include <chrono>
#include <format>
#include <memory>
#include <string>
#include <string_view>

namespace ipm::sys {

auto ThermalPerformance::thermalPerformanceInfo()
  -> std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom,
    ui::pages::RowStaticError>> {

  auto Rows = std::vector<std::variant<ui::pages::RowStatic,
    ui::pages::RowDynamic,
    ui::pages::RowCustom,
    ui::pages::RowStaticError>>();

  constexpr auto k_Base = "/sys/class/hwmon";

  auto BaseDir = utils::Directory(k_Base);
  if (!BaseDir.isOpen()) {
    return Rows;
  }

  BaseDir.forEachChild([&Rows](std::string_view HwmonName) -> void {
    if (!HwmonName.starts_with("hwmon")) {
      return;
    }

    auto HwmonPath = std::string(k_Base) + "/" + std::string(HwmonName);

    auto NameFile = utils::File(HwmonPath + "/name");
    auto DeviceName = std::string(HwmonName);
    if (NameFile.isRegular()) {
      auto NameRaw = NameFile.read();
      if (NameRaw.has_value() && !NameRaw->empty()) {
        DeviceName = std::move(NameRaw.value());
      }
    }

    auto HwmonDir = utils::Directory(HwmonPath);
    if (!HwmonDir.isOpen()) {
      return;
    }

    HwmonDir.forEachChild([&](std::string_view Filename) -> void {
      if (!Filename.ends_with("_input") || !Filename.starts_with("temp")) {
        return;
      }

      auto InputPath = HwmonPath + "/" + std::string(Filename);

      static constexpr auto k_MillidegToDegDivisor = 1000.0F;

      static constexpr auto k_TempValueBufferSize = 16U;

      auto LabelFilename = std::string(Filename);
      LabelFilename.replace(
        LabelFilename.find("_input"), k_TempValueBufferSize, "_label");
      auto LabelFile = utils::File(HwmonPath + "/" + LabelFilename);

      std::string Label;
      if (LabelFile.isRegular()) {
        auto LabelRaw = LabelFile.read();
        if (LabelRaw.has_value() && !LabelRaw->empty()) {
          Label = DeviceName + " \xe2\x80\x94 " + LabelRaw.value();
        } else {
          Label = DeviceName;
        }
      } else {
        Label = DeviceName;
      }

      auto TempFile = utils::File(InputPath);
      if (!TempFile.isRegular()) {
        Rows.emplace_back(std::make_tuple(std::move(Label)));
        return;
      }
      auto Probe = TempFile.read();
      if (!Probe.has_value()) {
        Rows.emplace_back(std::make_tuple(std::move(Label)));
        return;
      }

      auto CapturedPath = std::move(InputPath);

      auto File = utils::File(std::move(CapturedPath));

      auto Updater = [File = std::move(File)]() -> std::string {
        if (!File.isRegular()) {
          return "N/A";
        }
        auto Raw = File.read();
        if (!Raw.has_value()) {
          return Raw.value();
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
      };

      Rows.emplace_back(std::make_tuple(std::move(Label), std::move(Updater)));
    });
  });

  return Rows;
}

} // namespace ipm::sys
