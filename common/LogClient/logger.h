#pragma once
#include <iostream>
#include <sstream>
#include <exception>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ipc_message_queue_backend.hpp>
#include <boost/log/utility/ipc/reliable_message_queue.hpp>
#include <boost/log/utility/ipc/object_name.hpp>
#include <boost/log/utility/open_mode.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <string_view>

namespace FlexRP{

	namespace logging = boost::log;
	namespace expr = boost::log::expressions;
	namespace attrs = boost::log::attributes;
	namespace sinks = boost::log::sinks;
	namespace keywords = boost::log::keywords;

	//[ example_sinks_ipc_logger
	BOOST_LOG_ATTRIBUTE_KEYWORD(a_timestamp, "TimeStamp", attrs::local_clock::value_type)

	using queue_t   = logging::ipc::reliable_message_queue ;
	using backend_t = sinks::text_ipc_message_queue_backend< queue_t > ;
	using sink_t    = sinks::synchronous_sink< backend_t >;

	class Logger {

	public:
		Logger();

		void info(std::string_view arg);
		void debug(std::string_view arg);
		void error(std::string_view arg);
		void warn(std::string_view arg);
		void critical(std::string_view arg);
		
	private:
	    boost::log::sources::logger m_logger;
		boost::shared_ptr<sink_t> sink;
	};
}