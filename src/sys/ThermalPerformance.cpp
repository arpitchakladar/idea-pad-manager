#include "sys/ThermalPerformance.hpp"
#include "sys/utils/File.hpp"
#include "sys/utils/FileSystem.hpp"
#include "ui/pages/Page.hpp"
#include <chrono>
#include <format>
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

      // Check the temp file is actually readable before committing a row
      auto TempFile = utils::File(InputPath);
      if (!TempFile.isRegular()) {
        Rows.emplace_back(std::make_tuple(std::move(Label)));
        return;
      }
      // Probe once to make sure it's valid — avoids a dynamic row that
      // immediately shows an error on first render
      auto Probe = TempFile.read();
      if (!Probe.has_value()) {
        Rows.emplace_back(std::make_tuple(std::move(Label)));
        return;
      }

      // Capture InputPath by value into the lambda — Label is moved into
      // the tuple so we capture a copy here before the move
      auto CapturedPath = std::move(InputPath);

      // Shared mutable state for the throttle — captured by value via
      // shared_ptr so the lambda stays copyable
      struct Cache {
        std::string Value;
        std::chrono::time_point<std::chrono::steady_clock> LastRead;
      };
      auto SharedCache = std::make_shared<Cache>();
      // Seed the cache with the probe result so first render is instant
      {
        long Millideg = 0;
        try {
          Millideg = std::stol(Probe.value());
        } catch (...) {
        }
        if (Millideg > 0) {
          SharedCache->Value = std::format("{:.1f}\xC2\xB0"
                                           "C",
            static_cast<float>(Millideg) / k_MillidegToDegDivisor);
        } else {
          SharedCache->Value = "N/A";
        }
        SharedCache->LastRead = std::chrono::steady_clock::now();
      }

      auto Updater = [CapturedPath, SharedCache]() -> std::string {
        auto Now = std::chrono::steady_clock::now();
        if (Now - SharedCache->LastRead < std::chrono::seconds(1)) {
          return SharedCache->Value;
        }

        auto File = utils::File(CapturedPath);
        if (!File.isRegular()) {
          SharedCache->Value = "N/A";
          SharedCache->LastRead = Now;
          return SharedCache->Value;
        }
        auto Raw = File.read();
        if (!Raw.has_value()) {
          SharedCache->Value = "N/A";
          SharedCache->LastRead = Now;
          return SharedCache->Value;
        }

        long Millideg = 0;
        try {
          Millideg = std::stol(Raw.value());
        } catch (...) {
          SharedCache->Value = "N/A";
          SharedCache->LastRead = Now;
          return SharedCache->Value;
        }

        SharedCache->Value = std::format("{:.1f}\xC2\xB0"
                                         "C",
          static_cast<float>(Millideg) / k_MillidegToDegDivisor);
        SharedCache->LastRead = Now;
        return SharedCache->Value;
      };

      Rows.emplace_back(std::make_tuple(std::move(Label), std::move(Updater)));
    });
  });

  return Rows;
}

} // namespace ipm::sys
