#include "Flexrp_xml.h"
#include <spdlog/spdlog.h>
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
#include "flexrpsharedmemory.h"
#include "pugixml.hpp"

namespace FlexRP {

Session::Session(TcpSocket t_socket)
    : m_socket(std::move(t_socket)),
      m_fileSize(0)

{}

void Session::doRead() {
  auto self = shared_from_this();
  async_read_until(m_socket, m_requestBuf_, "\n\n",
                   [this, self](boost::system::error_code ec, size_t bytes) {
                     if (!ec)
                       processRead(bytes);
                     else
                       handleError(__FUNCTION__, ec);
                   });
}

void Session::processRead(size_t t_bytesTransferred) {
  spdlog::info("{}({}), in_avail = {}, size = {}, max_size = {}.", __FUNCTION__,
               t_bytesTransferred, m_requestBuf_.in_avail(),
               m_requestBuf_.size(), m_requestBuf_.max_size());

  std::istream requestStream(&m_requestBuf_);
  readData(requestStream);

  auto pos = m_fileName.find_last_of('\\');
  if (pos != std::string::npos) m_fileName = m_fileName.substr(pos + 1);

  createFile();

  // write extra bytes to file
  do {
    requestStream.read(m_buf.data(), static_cast<long>(m_buf.size()));
    spdlog::info("{} write {} bytes.", __FUNCTION__, requestStream.gcount());
    m_outputFile.write(m_buf.data(), requestStream.gcount());
  } while (requestStream.gcount() > 0);

  auto self = shared_from_this();
  m_socket.async_read_some(
      boost::asio::buffer(m_buf.data(), m_buf.size()),
      [this, self](boost::system::error_code ec, size_t bytes) {
        if (!ec)
          doReadFileContent(bytes);
        else
          handleError(__FUNCTION__, ec);
      });
}

void Session::readData(std::istream &stream) {
  stream >> m_fileName;
  stream >> m_fileSize;
  stream.read(m_buf.data(), 2);
  config_filename = m_fileName;

  spdlog::info("{} size is {}, tellg = {}", m_fileName, m_fileSize,
               stream.tellg());
}

void Session::createFile() {
  m_outputFile.open(m_fileName, std::ios_base::binary);
  if (!m_outputFile) {
    spdlog::info("{}: Failed to create {}", __LINE__, m_fileName);
    return;
  }
}

void Session::doReadFileContent(size_t t_bytesTransferred) {
  if (t_bytesTransferred > 0) {
    m_outputFile.write(m_buf.data(),
                       static_cast<std::streamsize>(t_bytesTransferred));

    spdlog::info("{} recv {} bytes", __FUNCTION__, m_outputFile.tellp());

    if (m_outputFile.tellp() >= static_cast<std::streamsize>(m_fileSize)) {
      spdlog::info("Received file: {}", m_fileName);
      return;
    }
  }
  auto self = shared_from_this();
  m_socket.async_read_some(
      boost::asio::buffer(m_buf.data(), m_buf.size()),
      [this, self](boost::system::error_code /*ec*/, size_t bytes) {
        doReadFileContent(bytes);
      });
}

void Session::handleError(std::string const &t_functionName,
                          boost::system::error_code const &t_ec) {
  spdlog::info("{} in {} due to {}", __FUNCTION__, t_functionName,
               t_ec.message());
}

Server::Server(IoService &t_ioService, short t_port,
               std::string const &t_workDirectory)
    : m_socket(t_ioService),
      m_acceptor(t_ioService, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(),
                                  static_cast<unsigned short>(t_port))),
      m_workDirectory(t_workDirectory) {
  createWorkDirectory();

  doAccept();
}

void Server::doAccept() {
  m_acceptor.async_accept(m_socket, [this](boost::system::error_code ec) {
    if (!ec) std::make_shared<Session>(std::move(m_socket))->start();
    // Enable this statement for an infinite loop
    // doAccept();
  });
}

void Server::createWorkDirectory() {
  using namespace boost::filesystem;
  auto currentPath = path(m_workDirectory);
  if (!exists(currentPath) && !create_directory(currentPath))
    spdlog::error("Coudn't create working directory: {}", m_workDirectory);
  current_path(currentPath);
}

void deserialize(Flexrp_configuration &fcg) {
  // Read and write from the stream the config file
  const std::string config_path{"/tmp/config"};

  // Read the config file
  try {
    boost::asio::io_service ioService;

    Server server(ioService, 8080, config_path);

    ioService.run();

  } catch (std::exception &e) {
    spdlog::error("Exception: {}", e.what());
  }

  // Rebuild the full path
  const std::string full_path{config_path + "/" + config_filename};

  if (!boost::filesystem::exists(full_path)) {
    spdlog::error("{} doesn't exist...", full_path.c_str());
    return;
  }

  pugi::xml_document doc;
  if (!doc.load_file(full_path.c_str())) {
    std::runtime_error("FlexRP configuration file not found");
  }

  pugi::xml_node nodes = doc.child("FleXReconPipelineConfiguration");

  // Start filling the config struct
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

void run_processes(const Flexrp_configuration &fcg) {
  // Remove shared memory on construction and destruction
  namespace bip = boost::interprocess;
  struct shm_remove {
    shm_remove() { bip::shared_memory_object::remove("FlexRPSharedMemory"); }
    ~shm_remove() { bip::shared_memory_object::remove("FlexRPSharedMemory"); }
  } remover;

  // Create a shared memory for the workers properties
  namespace bp = boost::process;
  try {
    bp::group g;
    const std::string connect_port{"tcp://localhost:555"};
    const std::string bind_port{"tcp://*:555"};
    size_t count{};

    // Launch readers
    std::for_each(
        fcg.readers.cbegin(), fcg.readers.cend(), [&](const Readers &r) {
          auto exec = bp::search_path(r.name.c_str());
          auto arg1 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count)
                          : bind_port + std::to_string(5 + count);
          auto arg2 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count + 1)
                          : bind_port + std::to_string(5 + count + 1);

          spdlog::debug("{} {} {}", exec.c_str(), arg1, arg2);

          bp::spawn(exec, arg1, arg2, g);
          ++count;
        });

    // Launch the workers
    std::for_each(fcg.recon_modules.cbegin(), fcg.recon_modules.cend(),
                  [&](const Recon_modules &rc) {
                    auto exec = bp::search_path(rc.name.c_str());
                    auto arg1 = (count + 1) % 2 == 1
                                    ? connect_port + std::to_string(5 + count)
                                    : bind_port + std::to_string(5 + count);
                    auto arg2 =
                        (count + 1) % 2 == 1
                            ? connect_port + std::to_string(5 + count + 1)
                            : bind_port + std::to_string(5 + count + 1);

                    spdlog::debug("{} {} {}", exec.c_str(), arg1, arg2);

                    if (!rc.properties.empty()) {
                      // spdlog::info("{}", rc.name.data());
                      std::vector<std::string> v;

                      for (auto e : rc.properties) {
                        spdlog::debug("{} : {}", e.name, e.value);
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
          auto exec = bp::search_path(w.name.c_str());
          auto arg1 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count)
                          : bind_port + std::to_string(5 + count);
          auto arg2 = (count + 1) % 2 == 1
                          ? connect_port + std::to_string(5 + count + 1)
                          : bind_port + std::to_string(5 + count + 1);

          spdlog::debug("{} {}", exec.c_str(), arg1);

          bp::spawn(exec, arg1, g);
          // ++count;
        });

    g.wait();

  } catch (std::exception &e) {
    spdlog::error("{}", e.what());
  }
}

}  // namespace FLEXRP
