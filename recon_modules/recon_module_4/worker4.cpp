#include "worker4.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <spdlog/spdlog.h>
#include "zhelpers.hpp"
#include "ThreadPool.h"

namespace FlexRP {

Worker4::Worker4(const char *protocol) : Module_Sink_1(protocol) {}

int FlexRP::Worker4::process() {
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

    spdlog::info("I am the last worker in the chain...");

    spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name,
                 h.userParameters->userParameterLong[0].value);

    //*** Message body
    receiver.recv(&body_msg);
    auto acq = static_cast<complex_float_t *>(body_msg.data());

    spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));

    // Send final data to the client
    sender_cl.send(message);
    sender_cl.send(body_msg);
  }

  return EXIT_SUCCESS;
}

}  // namespace FleXRP
