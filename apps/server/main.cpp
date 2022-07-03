#include "Flexrp_xml.h"
#include <spdlog/spdlog.h>
#include <exception>

int main() {
  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::info("Flex Recon Pipeline started...");
  using namespace FlexRP;
  while (true) {
    Flexrp_configuration fcg;
    try {
      deserialize(fcg);
      run_processes(fcg);
    } catch (const std::exception& e) {
      spdlog::error("Exception: {}", e.what());
    }
  }
}
