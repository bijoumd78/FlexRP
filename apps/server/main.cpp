#include "Flexrp_xml.h"
#include <boost/process.hpp>
#include "logger.h"
#include <exception>
#include <sstream>
#include <iostream>

int main() {
 
   // Launch central log server
   namespace bp = boost::process;
   std::vector<boost::filesystem::path> path{boost::filesystem::current_path()};
   const auto logServerPath = bp::search_path("LogServer", path);
   if (logServerPath.empty())
   {
       std::cout << "Failed to load central log server : " << logServerPath << std::endl;
       return EXIT_FAILURE;
   }

   bp::child c(logServerPath);
   if (c.running()){

        using namespace FlexRP;
        static Logger log;
        log.info( "Flex Recon Pipeline started...");
 
      while (true) {
        Flexrp_configuration fcg;
        try {
            // Read and write from the stream the config file
           boost::filesystem::current_path(Server::getWorkingDirectory());
           const std::string config_path{"./config"};
           boost::asio::io_service ioService;
           Server server(log, ioService, 8080, config_path);
           ioService.run();

           Server::deserialize(log, fcg);
           Server::run_processes(log, fcg);
        } 
        catch (const std::exception& e) {
            std::stringstream ss;
            ss << "Exception: " <<  e.what();
            log.critical(ss.str());
        }
      }
  }

   c.wait(); //wait for the process to exit

   return c.exit_code();
}
