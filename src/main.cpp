#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "ui/app.hpp"

auto main(int argc, char *argv[]) -> int {
#ifdef DEBUG_LOG_PATH
  auto FileSink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(DEBUG_LOG_PATH, true);
  auto Logger = std::make_shared<spdlog::logger>("debugging-logger", FileSink);
  spdlog::set_default_logger(Logger);
  spdlog::flush_on(spdlog::level::info);
#endif

  ipm::ui::App App;
  App.setup();
  App.run();
  App.stop();

  return 0;
}
