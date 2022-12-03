#include "configuration.h"
#include <boost/algorithm/string.hpp>
#include <json/json.h>
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
        // Close file
        ifs.close();
        std::cout << errs << std::endl;
        return;
    }

    // Close file
    ifs.close();

    // Parse config file
    m_loggingToConsoleEnabled = root.isMember("writeToConsole") ? root["writeToConsole"].asBool() : m_loggingToConsoleEnabled;
    m_loggingToFileEnabled    = root.isMember("writeToFile")    ? root["writeToFile"].asBool()    : m_loggingToFileEnabled;

    const auto& arr = root["channelType"];
    // Iterate over the channelType array
    for(Json::Value::ArrayIndex i = 0, SIZE = arr.size(); i != SIZE; ++i)
    {
        if ( arr[i].isMember("type") && boost::iequals( arr[i]["type"].asString(), std::string{ "file" } ))
        {
            m_file.loggingPattern = arr[i].isMember("loggingPattern")  ? arr[i]["loggingPattern"].asString()  : m_file.loggingPattern;
            m_file.path           = arr[i].isMember("path")            ? arr[i]["path"].asString()            : m_file.path;
            m_file.maxSize        = arr[i].isMember("rotationInBytes") ? arr[i]["rotationInBytes"].asUInt64() : m_file.maxSize;
            m_file.maxNumberFiles = arr[i].isMember("maxNumberFiles")  ? arr[i]["maxNumberFiles"].asUInt64()  : m_file.maxNumberFiles;
        }
        else
        {
            throw std::invalid_argument("Invalid channel type");
        }
    }
}

std::string Configuration::getFilePath() const              { return m_file.path;              }
std::string Configuration::getFileLoggingPattern() const    { return m_file.loggingPattern;    }
size_t Configuration::getFileMaxSize() const                { return m_file.maxSize;           }
size_t Configuration::getFileMaxNumberFiles() const         { return m_file.maxNumberFiles;    }

bool Configuration::isLoggingToConsoleEnabled() const
{
    return m_loggingToConsoleEnabled;
}

bool Configuration::isLoggingToFileEnabled() const
{
    return m_loggingToFileEnabled;
}
}
