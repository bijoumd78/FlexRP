#include "Flexrp_xml.h"
#include "logger.h"
#include <exception>

int main() {
 
  // Load logging configuration file
  common::logger::Logger log("logConfig.json");

  FL_INFO("Flex Recon Pipeline started...");  
  using namespace FlexRP;
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
      spdlog::error("Exception: {}", e.what());
    }
  }
}
