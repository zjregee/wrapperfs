#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "common/config.h"

namespace logger {

void init(const std::string &log_file);
void trace(const std::string &message);
void debug(const std::string &message);
void info(const std::string &message);
void warn(const std::string &message);
void error(const std::string &message);

}