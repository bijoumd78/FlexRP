#include "Flexrp_xml.h"
#include <spdlog/spdlog.h>
#include <exception>

int main() {
  // Set global log level to debug
  spdlog::set_level(spdlog::level::debug); 

  spdlog::info("Flex Recon Pipeline started...");
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
