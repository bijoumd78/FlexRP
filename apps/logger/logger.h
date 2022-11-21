#pragma once
#include "configuration.h"
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>
#include <memory>


namespace common::logger {

    class Logger
    {
     public:
        Logger() = delete;
        // Dependency injection
        explicit Logger(std::string_view config);

        static void info(const std::string& msg);
        static void debug(const std::string& msg);
        static void warn(const std::string& msg);
        static void error(const std::string& msg);
        static void critical(const std::string& msg);

     private:
            spdlog::level::level_enum convertStringToLoggingLevel(const std::string& level)const;
            std::unique_ptr<Configuration> p_config;
            inline static std::shared_ptr<spdlog::async_logger> p_logger {nullptr};
    };
}

#define FL_INFO(msg)     common::logger::Logger::info(msg)
#define FL_DEBUG(msg)    common::logger::Logger::debug(msg)
#define FL_WARN(msg)     common::logger::Logger::warn(msg)
#define FL_ERROR(msg)    common::logger::Logger::error(msg)
#define FL_CRITICAL(msg) common::logger::Logger::critical(msg)