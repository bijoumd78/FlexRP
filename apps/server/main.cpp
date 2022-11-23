#include "Flexrp_xml.h"
#include "logger.h"
#include <exception>

int main() {
 
    using namespace FlexRP;

  // Load logging configuration file
  Logger log("logConfig.json");

  Logger::info("Flex Recon Pipeline started...");
 
  while (true) {
    Flexrp_configuration fcg;
    try {
        // Read and write from the stream the config file
       boost::filesystem::current_path(Server::getWorkingDirectory());
       const std::string config_path{"./config"};
       boost::asio::io_service ioService;
       Server server(ioService, 8080, config_path);
       ioService.run();

      Server::deserialize(fcg);
      Server::run_processes(fcg);
    } 
    catch (const std::exception& e) {
      Logger::error("Exception: {}", e.what());
    }
  }
}
