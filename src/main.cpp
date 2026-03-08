#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "ui/app.hpp"

int main(int argc, char *argv[]) {
#ifdef DEBUG_LOG_PATH
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(DEBUG_LOG_PATH, true);
	auto logger = std::make_shared<spdlog::logger>("debugging-logger", file_sink);
	spdlog::set_default_logger(logger);
	spdlog::flush_on(spdlog::level::info);
#endif
	UI::App app;
	app.setup();
	app.run();

	return 0;
}
