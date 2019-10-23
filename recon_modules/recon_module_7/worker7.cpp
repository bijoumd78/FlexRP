#include "worker7.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <spdlog/spdlog.h>
#include "flexrpsharedmemory.h"
#include "zhelpers.hpp"

namespace FlexRP {

Worker7::Worker7(const char *protocol1, const char *protocol2)
    : Module_Worker_1(protocol1, protocol2) {}

int FlexRP::Worker7::process() {
  //  Process messages from both sockets
  while (true) {
    zmq::poll(&items[0], 2, -1);

    if (items[0].revents & ZMQ_POLLIN) {
      receiver.recv(&message);

      // Deserialize the header
      ISMRMRD::IsmrmrdHeader h;
      try {
        ISMRMRD::deserialize(static_cast<char *>(message.data()), h);
      } catch (...) {
        spdlog::error("Failed to parse incoming ISMRMRD Header");
      }

      spdlog::info("I am worker 7");

      spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name,
                   h.userParameters->userParameterLong[0].value);

      //*** Message body
      receiver.recv(&body_msg);
      auto acq = static_cast<complex_float_t *>(body_msg.data());

      spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));

      // Get properties (name, value)
      constexpr size_t n = 4;
      std::vector<std::string> property;
      FlexRP::FlexRPSharedMemory::getReconmoduleProperty(property, n);
      for (size_t e = 0; e < n; e += 2) {
        spdlog::info("Property(name, value)  {} : {}", property[e],
                     property[e + 1]);
      }

      //  Do the work
      s_sleep(1000);

      sender.send(message);
      sender.send(body_msg);
    }
    //  Any waiting controller command acts as 'KILL'
    if (items[1].revents & ZMQ_POLLIN) {
      break;
    }
  }

  return EXIT_SUCCESS;
}

}  // namespace FleXRP
