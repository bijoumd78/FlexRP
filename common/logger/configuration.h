#pragma once
#include <string>

namespace FlexRP {

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
            std::string loggingLevel{ "info" };
            std::string loggingPattern{"%Y-%m-%d %H:%M:%S [%t] %^[%l]%$ %v"};
            std::string timeZone{"local"}; // Not being used
        }m_console;

        struct{
            std::string loggingLevel{ "info" };
            std::string path{ "logs/mylog.txt" };
            std::string loggingPattern{ "%Y-%m-%d %H:%M:%S [%t] %^[%l]%$ %v" };
            size_t maxSize{ 1024 };
            size_t maxNumberFiles{10};
            std::string timeZone{"local"}; // Not being used
        }m_file;
    };

}
