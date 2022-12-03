#include "Flexrp_xml.h"
#include "flexrpsharedmemory.h"
#include "pugixml.hpp"
#include <algorithm>
#include <boost/asio/read_until.hpp>
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/process.hpp>
#include <boost/unordered_map.hpp>
#include <functional>
#include <stdexcept>
#include <sstream>


namespace FlexRP {

Session::Session(TcpSocket t_socket)
    : m_socket(std::move(t_socket)),
      m_buf{},
      m_fileSize(0)
{}

void Session::doRead(Logger& log) {
  auto self = shared_from_this();
  async_read_until(m_socket, m_requestBuf_, "\n\n",
                   [this, self, &log](boost::system::error_code ec, size_t bytes) {
                     if (!ec)
                       processRead(log, bytes);
                     else
                       handleError(log, __FUNCTION__, ec);
                   });
}

void Session::processRead(Logger& log, size_t t_bytesTransferred) {
    std::stringstream ss;

    ss << __FUNCTION__ << "(" << t_bytesTransferred << ") in_avail = " << m_requestBuf_.in_avail() 
        << " size = " << m_requestBuf_.size() << " max_size = " << m_requestBuf_.max_size();
    log.info(ss.str());

  std::istream requestStream(&m_requestBuf_);
  readData(log, requestStream);

  auto pos = m_fileName.find_last_of('\\');

  if (pos != std::string::npos) 
      m_fileName = m_fileName.substr(pos + 1);

  createFile(log);

  // write extra bytes to file
  do {
    requestStream.read(m_buf.data(), static_cast<long>(m_buf.size()));

    // Reset string stream
    ss.str("");
    ss.clear();
    ss << __FUNCTION__ << " write " << requestStream.gcount() << " bytes.";
    log.info(ss.str());

    m_outputFile.write(m_buf.data(), requestStream.gcount());
  } while (requestStream.gcount() > 0);

  auto self = shared_from_this();
  m_socket.async_read_some(
      boost::asio::buffer(m_buf.data(), m_buf.size()),
      [this, self, &log](boost::system::error_code ec, size_t bytes) {
        if (!ec)
          doReadFileContent(log, bytes);
        else
          handleError(log, __FUNCTION__, ec);
      });
}

void Session::readData(Logger& log, std::istream &stream) {
  stream >> m_fileName;
  stream >> m_fileSize;
  stream.read(m_buf.data(), 2);

  std::stringstream ss;
  ss << m_fileName << " size is " << m_fileSize << ", tellg = " << stream.tellg();
  log.info(ss.str());
}

void Session::createFile(Logger& log) {
  m_outputFile.open(m_fileName, std::ios_base::binary);
  if (!m_outputFile) {

    std::stringstream ss;
    ss << __LINE__ << ": Failed to create " << m_fileName;
    log.info(ss.str());
    return;
  }
}

void Session::doReadFileContent(Logger& log, size_t t_bytesTransferred) {
  if (t_bytesTransferred > 0) {
    m_outputFile.write(m_buf.data(),
                       static_cast<std::streamsize>(t_bytesTransferred));

    std::stringstream ss;
    ss << __FUNCTION__ << " recv " << m_outputFile.tellp() << " bytes";
    log.info(ss.str());

    if (m_outputFile.tellp() >= static_cast<std::streamsize>(m_fileSize)) {

      // Reset string stream
        ss.str("");
        ss.clear();
        ss << "Received file: " << m_fileName;
      log.info(ss.str());
      return;
    }
  }
  auto self = shared_from_this();
  m_socket.async_read_some(
      boost::asio::buffer(m_buf.data(), m_buf.size()),
      [this, self, &log](boost::system::error_code, size_t bytes) {
        doReadFileContent(log, bytes);
      });
}

void Session::handleError(Logger& log, std::string const &t_functionName,
                          boost::system::error_code const &t_ec) {

    std::stringstream ss;
    ss << __FUNCTION__ << " in " << t_functionName << " due to " << t_ec.message();
    log.info(ss.str());
}

Server::Server(Logger& log, IoService &t_ioService, short t_port,
               std::string const &t_workDirectory)
    : m_socket(t_ioService),
      m_acceptor(t_ioService, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(),
                                  static_cast<unsigned short>(t_port))),
     m_configFileDir(t_workDirectory)
{
  createWorkDirectory(log);
  doAccept(log);
}

void Server::doAccept(Logger& log) {
  m_acceptor.async_accept(m_socket, [this, &log](boost::system::error_code ec) {
    if (!ec) std::make_shared<Session>(std::move(m_socket))->start(log);
    // Enable this statement for an infinite loop
    // doAccept();
  });
}

void Server::createWorkDirectory(Logger& log) {
  using namespace boost::filesystem;
   auto currentPath = path(m_configFileDir);
  if (!exists(currentPath) && !create_directory(currentPath))
  {
      std::stringstream ss;
      ss << "Coudn't create working directory: " << m_configFileDir;
      log.error(ss.str());
  }
  current_path(currentPath);
}

void Server::deserialize(Logger& log, Flexrp_configuration &fcg) {
  // Rebuild the full path
  const auto full_path{boost::filesystem::current_path().append("/" + Session::getFilename())};

  if (!boost::filesystem::exists(full_path)) {
    std::stringstream ss;
    ss << full_path.string() << " doesn't exist...";
    log.error(ss.str());
    return;
  }

  pugi::xml_document doc;
  if (!doc.load_file(full_path.c_str())) {
    std::runtime_error("FlexRP configuration file not found");
  }

  pugi::xml_node nodes = doc.child("FleXReconPipelineConfiguration");

  pugi::xml_node reader = nodes.child("reader");
  while (reader) {
    Readers r;
    r.name = reader.child_value("name");
    fcg.readers.emplace_back(r);
    reader = reader.next_sibling("reader");
  }

  pugi::xml_node writer = nodes.child("writer");
  while (writer) {
    Writers w;
    w.name = writer.child_value("name");
    fcg.writers.push_back(w);
    writer = writer.next_sibling("writer");
  }

  pugi::xml_node recon_module = nodes.child("ReconModule");
  while (recon_module) {
    Recon_modules rm;
    rm.name = recon_module.child_value("name");

    pugi::xml_node property = recon_module.child("property");
    while (property) {
      Recon_modules_Properties p;
      p.name = property.child_value("name");
      p.value = property.child_value("value");
      rm.properties.push_back(p);
      property = property.next_sibling("property");
    }

    fcg.recon_modules.push_back(rm);
    recon_module = recon_module.next_sibling("ReconModule");
  }
}

void Server::run_processes(Logger& log, const Flexrp_configuration &fcg) {
  // Remove shared memory on construction and destruction
  namespace bip = boost::interprocess;
  struct shm_remove {
    shm_remove() { bip::shared_memory_object::remove("FlexRPSharedMemory"); }
    ~shm_remove() { bip::shared_memory_object::remove("FlexRPSharedMemory"); }
  } remover;

  // Create a shared memory for the workers properties
  namespace bp = boost::process;

    bp::group g;
    const std::string connect_port{"tcp://localhost:555"};
    const std::string bind_port{"tcp://*:555"};
    size_t count{};

    // Get Server directory
    std::vector<boost::filesystem::path> path{m_workDirectory};

    std::stringstream ss;
    // Launch readers
    std::for_each(
        fcg.readers.cbegin(), fcg.readers.cend(), [&](const Readers &r) {
          const auto exec = bp::search_path(r.name.c_str(), path);
          const auto &arg1 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count)
                          : bind_port + std::to_string(5 + count);
          const auto &arg2 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count + 1)
                          : bind_port + std::to_string(5 + count + 1);

          ss << exec.string() << " " << arg1 << " " << arg2;
          log.debug(ss.str());
          // Reset string stream
          ss.str("");
          ss.clear();

          bp::spawn(exec, arg1, arg2, g);
          ++count;
        });

    // Launch the workers
    std::for_each(fcg.recon_modules.cbegin(), fcg.recon_modules.cend(),
                  [&](const Recon_modules &rc) {
                    const auto exec = bp::search_path(rc.name.c_str(), path);
                    const auto &arg1 = (count + 1) % 2 == 1
                                    ? connect_port + std::to_string(5 + count)
                                    : bind_port + std::to_string(5 + count);
                    const auto &arg2 =
                        (count + 1) % 2 == 1
                            ? connect_port + std::to_string(5 + count + 1)
                            : bind_port + std::to_string(5 + count + 1);

                    ss << exec.string() << " " << arg1 << " " << arg2;
                    log.debug(ss.str());
                    // Reset string stream
                    ss.str("");
                    ss.clear();

                    if (!rc.properties.empty()) {
                      log.info(rc.name.data());
                      std::vector<std::string> v;

                      for (auto e : rc.properties) {
                        ss << e.name << " : " << e.value;
                        log.debug(ss.str());
                        // Reset string stream
                        ss.str("");
                        ss.clear();

                        v.emplace_back(e.name);
                        v.emplace_back(e.value);
                      }

                      // Fill the shared memory
                      FlexRP::FlexRPSharedMemory::createSharedMemory(v);
                    }

                    bp::spawn(exec, arg1, arg2, g);
                    ++count;
                  });

    // Launch the writers
    std::for_each(
        fcg.writers.cbegin(), fcg.writers.cend(), [&](const Writers &w) {
          const auto exec = bp::search_path(w.name.c_str(), path);
          const auto &arg1 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count)
                          : bind_port + std::to_string(5 + count);
          const auto &arg2 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count + 1)
                          : bind_port + std::to_string(5 + count + 1);
          ss << exec.string() << " " << arg1;
          log.debug(ss.str());

          bp::spawn(exec, arg1, g);
        });

    g.wait();
}

}  // namespace FLEXRP
