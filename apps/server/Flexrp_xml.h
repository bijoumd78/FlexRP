#ifndef FLEXRP_XML_H
#define FLEXRP_XML_H

#include <array>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <logger.h>

namespace FlexRP {

struct Readers {
  std::string name;
};

struct Writers {
  std::string name;
};

struct Recon_modules_Properties {
  std::string name;
  std::string value;
};

struct Recon_modules {
  std::string name;
  std::vector<Recon_modules_Properties> properties;
};

struct Flexrp_configuration {
  std::vector<Readers> readers;
  std::vector<Writers> writers;
  std::vector<Recon_modules> recon_modules;
};

class Session : public std::enable_shared_from_this<Session> {
 public:
  using TcpSocket = boost::asio::ip::tcp::socket;

  explicit Session(TcpSocket t_socket);

  void start(Logger& log) { doRead(log); }

  static std::string getFilename() {
      return m_fileName;
  }

 private:
  void doRead(Logger& log);
  void processRead(Logger& log, size_t t_bytesTransferred);
  void createFile(Logger& log);
  void readData(Logger& log, std::istream& stream);
  void doReadFileContent(Logger& log, size_t t_bytesTransferred);
  void handleError(Logger& log, std::string const& t_functionName,
                   boost::system::error_code const& t_ec);

  TcpSocket m_socket;
  enum { MaxLength = 40960 };
  std::array<char, MaxLength> m_buf;
  boost::asio::streambuf m_requestBuf_;
  std::ofstream m_outputFile;
  size_t m_fileSize;
  inline static std::string m_fileName{};
};

class Server {
 public:
  using TcpSocket = boost::asio::ip::tcp::socket;
  using TcpAcceptor = boost::asio::ip::tcp::acceptor;
  using IoService = boost::asio::io_service;

  Server(Logger& log, IoService& t_ioService, short t_port,
         std::string const& t_workDirectory);

  static void deserialize(Logger& log, Flexrp_configuration& fcg);
  static void run_processes(Logger& log,const Flexrp_configuration& fcg);
  static boost::filesystem::path getWorkingDirectory(){ return m_workDirectory; }

 private:
  // Utility functions
  void doAccept(Logger& log);
  void createWorkDirectory(Logger& log);

  TcpSocket m_socket;
  TcpAcceptor m_acceptor;
  std::string m_configFileDir;
  inline static std::string config_filename{};
  inline static const auto m_workDirectory{boost::filesystem::current_path()};
};

}  // namespace FLEXRP

#endif  // FLEXRP_XML_H
