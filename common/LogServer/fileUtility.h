#pragma once
#include <filesystem>
#include <map>

namespace FlexRP {

namespace fs = std::filesystem;

class FileUtility
{
 public:
     void removeExcessFiles(const fs::path& path, const size_t maxNumberOfFilesAllowed) const;
     std::string getCurrentDataTime()const;

 private:
    size_t currentTotalNumberOfFiles(const std::filesystem::path& path) const;
    std::map<long long, fs::directory_entry, std::greater<>> currentFiles(const fs::path& path)const;
};

}

