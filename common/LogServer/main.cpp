/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 11/28/2022
 * Version: 0.0.1
 *****************************************************************************/
#include "configuration.h"
#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include <boost/log/utility/ipc/reliable_message_queue.hpp>
#include <boost/log/utility/ipc/object_name.hpp>
#include <boost/log/utility/open_mode.hpp>
#include <thread>
#include <stdexcept>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace fs = std::filesystem;
using namespace std::chrono_literals;

int main()
{
    try {

        // Load logging configuration file
        FlexRP::Configuration config("logConfig.json");
        // Check if the logging directory exist
        const auto path = config.getFilePath();
        auto root = fs::path{path.substr(0, path.find_last_of("/"))};
        if(root.empty()) { root = fs::path{"logs"}; }
        if (!fs::is_directory(root) || !fs::exists(root)) { fs::create_directory(root); }

        // Check number of files in the directory. Remove file older than 1 day
        config.removeFileIfExceedingMaxFileAllow(root, 24h /*1h + 20min*/);

        using queue_t = logging::ipc::reliable_message_queue;
        // Create a message_queue_type object that is associated with the interprocess
        // message queue named "ipc_message_queue".
        queue_t queue
        (
            keywords::name = logging::ipc::object_name(logging::ipc::object_name::user, "ipc_message_queue"),
            keywords::open_mode = logging::open_mode::open_or_create,
            keywords::capacity = 256,
            keywords::block_size = 1024,
            keywords::overflow_policy = queue_t::block_on_overflow
        );

        puts("Central logging server running...");

        // Keep reading log messages from the associated message queue and print them on the console.
        // queue.receive() will block if the queue is empty.
        std::string message;
        static unsigned short count{};
        std::ofstream logFile;
        fs::path fileName{ path };
        uintmax_t rotationSize{ config.getFileMaxSize() };
        while (queue.receive(message) == queue_t::succeeded)
        {
            // Check number of files in the directory. Remove file older than 1 day
            config.removeFileIfExceedingMaxFileAllow(root, 24h /*1h + 20min*/);

            if(config.isLoggingToConsoleEnabled()) { std::cout << message << std::endl; }

            if (config.isLoggingToFileEnabled())
            {
                // Write to file
                logFile.open(fileName, std::ios_base::app | std::ios::out);
                if (!logFile) { throw std::invalid_argument("Failed to create Log file..."); }

                // Check current file size
                if ( auto currentFileSize = fs::file_size(fileName); 
                     currentFileSize > rotationSize || 
                     (currentFileSize + message.size()) > rotationSize )
                {
                    logFile.close();
                    const auto fileNameString = fileName.string();
                    auto pathWithoutExt = fileNameString.substr(0, fileNameString.find_last_of("."));
                    const auto newFileName = pathWithoutExt + std::string{ "_" } + std::to_string(++count) + std::string{ ".txt" };
                    fs::rename(fileName, fs::path{ newFileName });
                }

                logFile << message << '\n';
                logFile.close();
            }

            // Clear the buffer for the next message
            message.clear();
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Main function caught: " << e.what() << std::endl;
    }

    return 0;
}