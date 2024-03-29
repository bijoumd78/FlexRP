#include "flexrp_client.h"
#include <spdlog/spdlog.h>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace FlexRP_client {

Client::Client(IoService& t_ioService,
               const TcpResolverIterator& t_endpointIterator,
               std::string const& t_path)
    : m_ioService(t_ioService),
      m_socket(t_ioService),
      m_endpointIterator(t_endpointIterator),
      m_buf{},
      m_path(t_path)
{
  doConnect();
  openFile(m_path);
}

void Client::openFile(std::string const& t_path) {
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

void Client::doConnect() {
  boost::asio::async_connect(
      m_socket, m_endpointIterator,
      [this](boost::system::error_code ec, TcpResolverIterator) {
        if (!ec) {
          writeBuffer(m_request);
        } else {
          spdlog::error(
              "Coudn't connect to host. Please run server or check network "
              "connection.");
          spdlog::error("Error: {}", ec.message());
        }
      });
}

void Client::doWriteFile(const boost::system::error_code& t_ec) {
  if (!t_ec) {
    if (m_sourceFile) {
      m_sourceFile.read(m_buf.data(), static_cast<long>(m_buf.size()));
      if (m_sourceFile.fail() && !m_sourceFile.eof()) {
        auto msg = "Failed while reading file";
        spdlog::error("{}", msg);
        throw std::fstream::failure(msg);
      }
      std::stringstream ss;
      ss << "Send " << m_sourceFile.gcount()
         << " bytes, total: " << m_sourceFile.tellg() << " bytes";

      spdlog::info("{}", ss.str());

      auto buf = boost::asio::buffer(
          m_buf.data(), static_cast<size_t>(m_sourceFile.gcount()));
      writeBuffer(buf);
    }
  } else {
    spdlog::error("Error: {}", t_ec.message());
  }
}

}  // namespace FlexRP_client
