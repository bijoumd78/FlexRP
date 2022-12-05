#pragma once
#include <string>
#include <filesystem>
#include <string_view>
#include <chrono>

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
        size_t      numberOfFilesInLogDirectory(const std::filesystem::path& path)const;

        template <typename Duration>
        void        removeFileIfExceedingMaxFileAllow(const std::filesystem::path& path, const Duration duration)
        {
             if (getFileMaxNumberFiles() > numberOfFilesInLogDirectory(path)) { removeFilesOlderThan(path, duration); }
        }

    private:
        bool m_loggingToConsoleEnabled = false;
        bool m_loggingToFileEnabled    = false;

        struct{
            std::string path{ "logs/mylog.txt" };
            size_t maxSize{ 1024 };
            size_t maxNumberFiles{1};
        }m_file;

 
         // Utility functions
         template <typename Duration>
         bool isOlderThan(const std::filesystem::path& path, const Duration duration)
         {
            auto lastwrite = std::filesystem::last_write_time(path);
            auto ftimeduration = lastwrite.time_since_epoch();
            auto nowduration = (std::chrono::system_clock::now() - duration).time_since_epoch();
            return std::chrono::duration_cast<Duration>(nowduration - ftimeduration).count() > 0;
         }

         template <typename Duration>
         void removeFilesOlderThan(const std::filesystem::path& path, const Duration duration)
         {
            try
            {
               if (std::filesystem::exists(path))
               {
                  if (isOlderThan(path, duration))
                  {
                     std::filesystem::remove(path);
                  }
                  else if(std::filesystem::is_directory(path))
                  { 
                     for (const auto & entry : std::filesystem::directory_iterator(path))
                     {
                       removeFilesOlderThan(entry.path(), duration);
                     }
                  }
               }
            }
            catch (const std::exception & ex)
            {
               std::cerr << ex.what() << std::endl;
            }
         }
    };
}

