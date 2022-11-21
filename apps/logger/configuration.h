#pragma once
#include <string>

namespace common::logger {

class Configuration
{
 public:
        Configuration() = delete;
        explicit Configuration(std::string_view configFile);

        void loadConfigFile(std::string_view configFile);

        std::string getConsoleLoggingLevel()const;
        std::string getConsoleLoggingPattern()const;
        std::string getConsoleTimeZone()const;

        std::string getFileLoggingLevel()const;
        std::string getFilePath()const;
        std::string getFileLoggingPattern()const;
        std::string getFileTimeZone()const;
        size_t getFileMaxSize()const;
        size_t getFileMaxNumberFiles()const;

        inline static bool isLoggingToConsoleEnabled = false;
        inline static bool isLoggingToFileEnabled    = false;

    private:

        struct{
            inline static std::string loggingLevel{ "info" };
            inline static std::string loggingPattern{"%Y-%m-%d %H:%M:%S [%t] %^[%l]%$ %v"};
            inline static std::string timeZone{"local"};
        }m_console;

        struct{
            inline static std::string loggingLevel{ "info" };
            inline static std::string path{ "logs/mylog.txt" };
            inline static std::string loggingPattern{ "%Y-%m-%d %H:%M:%S [%t] %^[%l]%$ %v" };
            inline static size_t maxSize{ 1024 };
            inline static size_t maxNumberFiles{10};
            inline static std::string timeZone{"local"};
        }m_file;
    };

}
