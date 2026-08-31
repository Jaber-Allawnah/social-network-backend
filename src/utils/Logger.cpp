#include "Logger.hpp"

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Logger {
	void init() {
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/social_network.log");
		auto logger = std::make_shared<spdlog::logger>("social_network", fileSink);

		logger->set_level(spdlog::level::debug);
		logger->flush_on(spdlog::level::info);

		spdlog::set_default_logger(logger);
	}
}