#include "worker5.h"
#include "zhelpers.hpp"
#include "ThreadPool.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <logger.h>
#include <sstream>

namespace FlexRP {

Worker5::Worker5(const char *protocol) : Module_Sink_2(protocol) {}

int FlexRP::Worker5::process(Logger& log) {
  zmq::poll(&items[0], 2, -1);

  if (items[0].revents & ZMQ_POLLIN) {
    receiver.recv(&message);

    // Deserialize the header
    ISMRMRD::IsmrmrdHeader h;
    try {
      ISMRMRD::deserialize(static_cast<char *>(message.data()), h);
    } catch (...) {
        log.error("Failed to parse incoming ISMRMRD Header");
    }

    log.info("I am the last worker in the chain...");

    //*** Message body
    receiver.recv(&body_msg);
    auto acq = static_cast<complex_float_t *>(body_msg.data());
    std::stringstream ss;
    ss << "Data: " << real(acq[4]) << " " << imag(acq[4]);
    log.info(ss.str());

    // Send final data to the client
    sender_cl.send(message);
    sender_cl.send(body_msg);
  }

  return EXIT_SUCCESS;
}

}  // namespace FleXRP
