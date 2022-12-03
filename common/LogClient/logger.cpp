#pragma once
#include "logger.h"


namespace FlexRP{

Logger::Logger()
{
    try
    {
        // Create a sink that is associated with the interprocess message queue
        // named "ipc_message_queue".
        sink = boost::make_shared< sink_t >
            (
                keywords::name = logging::ipc::object_name(logging::ipc::object_name::user, "ipc_message_queue"),
                keywords::open_mode = logging::open_mode::open_or_create,
                keywords::capacity = 256,
                keywords::block_size = 1024,
                keywords::overflow_policy = queue_t::block_on_overflow
                );

        // Set the formatter
        sink->set_formatter
        (
            expr::stream << "[" << a_timestamp << "] " << expr::smessage
        );

        logging::core::get()->add_sink(sink);

        // Add the commonly used attributes, including TimeStamp, ProcessID and ThreadID
        logging::add_common_attributes();
    }
    catch (const std::exception& e)
    {
        std::cout << "Failure: " << e.what() << std::endl;
    }
}

    void Logger::info(std::string_view arg)     { BOOST_LOG(m_logger) << " [INFO] " <<  arg; }
    void Logger::debug(std::string_view arg)    { BOOST_LOG(m_logger) << " [DEBUG] " <<  arg; }
    void Logger::error(std::string_view arg)    { BOOST_LOG(m_logger) << " [ERROR] " <<  arg; }
    void Logger::warn(std::string_view arg)     { BOOST_LOG(m_logger) << " [WARNING] " <<  arg; }
    void Logger::critical(std::string_view arg) { BOOST_LOG(m_logger) << " [CRITICAL] " <<  arg; }
}