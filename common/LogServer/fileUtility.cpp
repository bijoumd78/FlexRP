#include "fileUtility.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <sstream>


namespace FlexRP {

    std::size_t FileUtility::currentTotalNumberOfFiles(const fs::path& path) const
    {
        using fs::directory_iterator;
        return std::distance(directory_iterator(path), directory_iterator{});
    }

    std::map<long long, fs::directory_entry, std::greater<>> FileUtility::currentFiles(const fs::path& path) const
    {
        using recursive_directory_iterator = fs::recursive_directory_iterator;
        namespace ch = std::chrono;
        std::map<long long, fs::directory_entry, std::greater<>> files;
        for (const auto& dirEntry : recursive_directory_iterator(path))
        {
            auto lastwrite = fs::last_write_time(dirEntry);
            auto ftimeduration = lastwrite.time_since_epoch();
            files.try_emplace(ch::duration_cast<ch::milliseconds>(ftimeduration).count(), dirEntry);
        }
        return files;
    }

    void FileUtility::removeExcessFiles(const fs::path& path, const size_t maxNumberOfFilesAllowed) const
    {
        if (currentTotalNumberOfFiles(path) < maxNumberOfFilesAllowed) return;
        // Remove old files
        auto files = currentFiles(path);
        for (auto itr = std::next(cbegin(files), maxNumberOfFilesAllowed - 1); itr != cend(files); ++itr)  
        {
            std::cout << "Warning: Deleting files: " << itr->second << std::endl;
            fs::remove(itr->second);
        }
    }

    std::string FileUtility::getCurrentDataTime() const
    {
        const boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y-%m-%d_%H_%M_%S_%f");
        std::stringstream date_stream;
        date_stream.imbue(std::locale(date_stream.getloc(), facet));
        date_stream << boost::posix_time::microsec_clock::local_time();
        return date_stream.str();
    }

}
