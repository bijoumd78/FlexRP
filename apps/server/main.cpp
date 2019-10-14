#include <spdlog/spdlog.h>
#include <exception>
#include "Flexrp_xml.h"

int main() {
  spdlog::info("Flex Recon Pipeline started...");
  using namespace FLEXRP;
  while (true) {
    Flexrp_configuration fcg;
    try {
      deserialize(fcg);
      run_processes(fcg);
    } catch (std::exception& e) {
      spdlog::error("Exception: {}", e.what());
    }
  }
}
