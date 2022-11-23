#include "worker5.h"
#include "zhelpers.hpp"
#include "ThreadPool.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <logger.h>

namespace FlexRP {

Worker5::Worker5(const char *protocol) : Module_Sink_2(protocol) {}

int FlexRP::Worker5::process() {
  zmq::poll(&items[0], 2, -1);

  if (items[0].revents & ZMQ_POLLIN) {
    receiver.recv(&message);

    // Deserialize the header
    ISMRMRD::IsmrmrdHeader h;
    try {
      ISMRMRD::deserialize(static_cast<char *>(message.data()), h);
    } catch (...) {
      Logger::error("Failed to parse incoming ISMRMRD Header");
    }

    Logger::info("I am the last worker in the chain...");

    /*Logger::info("{} is {}", h.userParameters->userParameterLong[0].name,
                 h.userParameters->userParameterLong[0].value);*/

    //*** Message body
    receiver.recv(&body_msg);
    auto acq = static_cast<complex_float_t *>(body_msg.data());

    Logger::info("Data:: {} {}", real(acq[4]), imag(acq[4]));

    // Send final data to the client
    sender_cl.send(message);
    sender_cl.send(body_msg);
  }

  return EXIT_SUCCESS;
}

}  // namespace FleXRP
