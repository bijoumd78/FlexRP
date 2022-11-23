#include "configuration.h"
#include <boost/algorithm/string.hpp>
#include <json/json.h>
#include <functional>
#include <fstream>
#include <iostream>

namespace FlexRP {

    Configuration::Configuration(std::string_view configFile)
    {
        // Load the config file only once per application run
        // This Lambda trick is equivalent to the call_once functionality but nicer.
        static auto _ = [this, &configFile]() { loadConfigFile(configFile); return 0; }();
        (void)_;
    }

    void Configuration::loadConfigFile(std::string_view configFile)
    {
        Json::Value root;
        std::ifstream ifs;



        ifs.open(configFile.data(), std::ios::in);
        if (ifs.fail()) 
        { 
            std::cout << "Unable to read the configuration file: " << configFile << std::endl;
            return;
        }

        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        if (JSONCPP_STRING errs; !parseFromStream(builder, ifs, &root, &errs)) 
        {
            std::cout << errs << std::endl;
            return;
        }

        // Parse config file
        isLoggingToConsoleEnabled = root.isMember("writeToConsole") ? root["writeToConsole"].asBool() : isLoggingToConsoleEnabled;
        isLoggingToFileEnabled    = root.isMember("writeToFile")    ? root["writeToFile"].asBool()    : isLoggingToFileEnabled;

        const auto& arr = root["channelType"];
        // Iterate over the channelType array
        for(Json::Value::ArrayIndex i = 0, SIZE = arr.size(); i != SIZE; ++i)
        {
            if ( arr[i].isMember("type") && boost::iequals( arr[i]["type"].asString(), std::string{ "console" } ) )
            {
                m_console.loggingLevel   = arr[i].isMember("loggingLevel")   ? arr[i]["loggingLevel"].asString()   : m_console.loggingLevel;
                m_console.loggingPattern = arr[i].isMember("loggingPattern") ? arr[i]["loggingPattern"].asString() : m_console.loggingPattern;
                m_console.timeZone       = arr[i].isMember("timeZone")       ? arr[i]["timeZone"].asString()       : m_console.timeZone;
            }
            else if ( arr[i].isMember("type") && boost::iequals( arr[i]["type"].asString(), std::string{ "file" } ))
            {
                m_file.loggingLevel   = arr[i].isMember("loggingLevel")    ? arr[i]["loggingLevel"].asString()    : m_file.loggingLevel;
                m_file.loggingPattern = arr[i].isMember("loggingPattern")  ? arr[i]["loggingPattern"].asString()  : m_file.loggingPattern;
                m_file.path           = arr[i].isMember("path")            ? arr[i]["path"].asString()            : m_file.path;
                m_file.maxSize        = arr[i].isMember("rotationInBytes") ? arr[i]["rotationInBytes"].asUInt64() : m_file.maxSize;
                m_file.maxNumberFiles = arr[i].isMember("maxNumberFiles")  ? arr[i]["maxNumberFiles"].asUInt64()  : m_file.maxNumberFiles;
                m_file.timeZone       = arr[i].isMember("timeZone")        ? arr[i]["timeZone"].asString()        : m_file.timeZone;
            }
             else
            {
                throw std::invalid_argument("Invalid channel type");
            }
        }
        
    }

    std::string Configuration::getConsoleLoggingLevel() const   { return m_console.loggingLevel;   }
    std::string Configuration::getConsoleLoggingPattern() const { return m_console.loggingPattern; }
    std::string Configuration::getConsoleTimeZone() const       { return m_console.timeZone;       }
    std::string Configuration::getFileLoggingLevel() const      { return m_file.loggingLevel;      }
    std::string Configuration::getFilePath() const              { return m_file.path;              }
    std::string Configuration::getFileLoggingPattern() const    { return m_file.loggingPattern;    }
    std::string Configuration::getFileTimeZone() const          { return m_file.timeZone;          }
    size_t Configuration::getFileMaxSize() const                { return m_file.maxSize;           }
    size_t Configuration::getFileMaxNumberFiles() const         { return m_file.maxNumberFiles;    }

}
