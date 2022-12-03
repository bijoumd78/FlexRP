#pragma once
#include <string>
#include <string_view>

namespace FlexRP {
class Configuration
{
 public:
        Configuration() = delete;
        explicit Configuration(std::string_view configFile);

        void loadConfigFile(std::string_view configFile);

        std::string getFilePath()const;
        size_t      getFileMaxSize()const;
        size_t      getFileMaxNumberFiles()const;
        bool        isLoggingToConsoleEnabled()const;
        bool        isLoggingToFileEnabled()const;

    private:

        bool m_loggingToConsoleEnabled = false;
        bool m_loggingToFileEnabled    = false;

        struct{
            std::string path{ "logs/mylog.txt" };
            size_t maxSize{ 1024 };
            size_t maxNumberFiles{10};
        }m_file;
    };
}

