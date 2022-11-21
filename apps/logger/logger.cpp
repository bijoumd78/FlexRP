#include "logger.h"
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <boost/algorithm/string.hpp>
#include <utility>

namespace common::logger {

    // Log level enum

Logger::Logger(std::string_view configFile):
    p_config{std::make_unique<Configuration>(configFile)}
{
     spdlog::init_thread_pool(8192, 1);

     // Set console's properties
     auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
     console_sink->set_level(convertStringToLoggingLevel(p_config->getConsoleLoggingLevel()));
     console_sink->set_pattern(p_config->getConsoleLoggingPattern());

     // Set file's properties
     auto rotatingFile_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(p_config->getFilePath(), 
         p_config->getFileMaxSize(), p_config->getFileMaxSize());
     rotatingFile_sink->set_level(convertStringToLoggingLevel(p_config->getFileLoggingLevel()));
     rotatingFile_sink->set_pattern(p_config->getFileLoggingPattern());

     // Collect ouput sinks
     std::vector<spdlog::sink_ptr> sinks;
     if (Configuration::isLoggingToConsoleEnabled) { sinks.push_back(console_sink);      }
     if (Configuration::isLoggingToFileEnabled)    { sinks.push_back(rotatingFile_sink); }

     p_logger = std::make_shared<spdlog::async_logger>("FLEX", sinks.begin(), sinks.end(), spdlog::thread_pool(), 
                                                        spdlog::async_overflow_policy::block);
     spdlog::register_logger(p_logger);
}

void Logger::info(const std::string& msg    ) { p_logger->info(msg);    }
void Logger::debug(const std::string& msg   ) { p_logger->debug(msg);   }
void Logger::warn(const std::string& msg    ) { p_logger->warn(msg);    }
void Logger::error(const std::string& msg   ) { p_logger->error(msg);   }
void Logger::critical(const std::string& msg) { p_logger->critical(msg);}

spdlog::level::level_enum Logger::convertStringToLoggingLevel(const std::string& level) const
{
    if(boost::iequals(level, std::string{"info"})    ) { return spdlog::level::info;    }
    if(boost::iequals(level, std::string{"debug"})   ) { return spdlog::level::debug;   }
    if(boost::iequals(level, std::string{"warning"}) ) { return spdlog::level::warn;    }
    if(boost::iequals(level, std::string{"error"})   ) { return spdlog::level::err;     }
    if(boost::iequals(level, std::string{"critical"})) { return spdlog::level::critical;}
    if(boost::iequals(level, std::string{"off"})     ) { return spdlog::level::off;     }
    return {};
}

}