#include "flexrp_client.h"
#include <spdlog/spdlog.h>
#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
//#include <boost/log/trivial.hpp>
//#include "client.h"

namespace FlexRP_client {
#if 0
void send_xml_config_file(const std::string & config_file)
{
    using boost::asio::ip::tcp;

    try
    {
        const char *port = "10000";
        const char *ip   = "localhost";
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(ip, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        tcp::socket socket(io_service);
        socket.connect(*endpoint_iterator);

        // Send the configuration file name
        boost::filesystem::path p (config_file.c_str());
        auto tmp = static_cast<std::string>(p.filename().c_str());
        spdlog::info("Config file name: {} and size: {}", tmp, tmp.size());

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(tmp, tmp.size()), ignored_error);


        //Send the contain of the file
        std::fstream file(config_file);
        const unsigned int buff_size = 65536;
        std::unique_ptr<char[]> buff(new char[buff_size]);
        unsigned int count = 0;
        spdlog::info("Sending configuration file...");

        while( !file.eof() ) {
            memset(buff.get(),0,buff_size);
            file.read(buff.get(),buff_size);
            boost::system::error_code ignored_error;
            auto len = file.gcount();
            boost::asio::write(socket, boost::asio::buffer(buff.get(), static_cast<size_t>(len)),
                               boost::asio::transfer_all(), ignored_error);
            count+=len;
        }

        file.close();
        spdlog::info("Sent {} bytes", count);
        spdlog::info("Done...");
    }
    catch (std::exception& e)
    {
        spdlog::error(e.what());
    }
}
#endif

Client::Client(IoService& t_ioService, TcpResolverIterator t_endpointIterator,
               std::string const& t_path)
    : m_ioService(t_ioService), m_socket(t_ioService),
      m_endpointIterator(t_endpointIterator), m_path(t_path)
{
    doConnect();
    openFile(m_path);
}


void Client::openFile(std::string const& t_path)
{
    m_sourceFile.open(t_path, std::ios_base::binary | std::ios_base::ate);
    if (m_sourceFile.fail())
        throw std::fstream::failure("Failed while opening file " + t_path);

    m_sourceFile.seekg(0, m_sourceFile.end);
    auto fileSize = m_sourceFile.tellg();
    m_sourceFile.seekg(0, m_sourceFile.beg);

    std::ostream requestStream(&m_request);
    boost::filesystem::path p(t_path);
    requestStream << p.filename().string() << "\n" << fileSize << "\n\n";
    spdlog::info("Request size: {}", m_request.size());
}


void Client::doConnect()
{
    boost::asio::async_connect(m_socket, m_endpointIterator,
                               [this](boost::system::error_code ec, TcpResolverIterator)
    {
        if (!ec) {
            writeBuffer(m_request);
        } else {
            spdlog::error("Coudn't connect to host. Please run server or check network connection.");
            spdlog::error("Error: {}", ec.message());
        }
    });
}


void Client::doWriteFile(const boost::system::error_code& t_ec)
{
    if (!t_ec) {
        if (m_sourceFile) {
            m_sourceFile.read(m_buf.data(), static_cast<long>(m_buf.size()));
            if (m_sourceFile.fail() && !m_sourceFile.eof()) {
                auto msg = "Failed while reading file";
                spdlog::error("{}", msg);
                throw std::fstream::failure(msg);
            }
            std::stringstream ss;
            ss << "Send " << m_sourceFile.gcount() << " bytes, total: "
               << m_sourceFile.tellg() << " bytes";

            spdlog::info("{}", ss.str());

            auto buf = boost::asio::buffer(m_buf.data(), static_cast<size_t>(m_sourceFile.gcount()));
            writeBuffer(buf);
        }
    } else {
        spdlog::error("Error: {}", t_ec.message());
    }
}

}
