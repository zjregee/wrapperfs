#include "utils/logger_routine.h"

namespace logger {

void init(const std::string &log_file) {
    auto file_logger = spdlog::basic_logger_mt("file_logger", log_file);
    spdlog::set_default_logger(file_logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

void trace(const std::string &message) {
    if (wrapperfs::ENABELD_LOG) {
        spdlog::trace(message);
    }
}

void debug(const std::string &message) {
    if (wrapperfs::ENABELD_LOG) {
        spdlog::debug(message);
    }
}

void info(const std::string &message) {
    if (wrapperfs::ENABELD_LOG) {
        spdlog::info(message);
    }
}

void warn(const std::string &message) {
    if (wrapperfs::ENABELD_LOG) {
        spdlog::warn(message);
    }
}

void error(const std::string &message) {
    if (wrapperfs::ENABELD_LOG) {
        spdlog::error(message);
    }
}

}