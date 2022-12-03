#include "worker1.h"
#include "zhelpers.hpp"
#include "ThreadPool.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <logger.h>
#include <sstream>

namespace FlexRP {

Worker1::Worker1(const char *protocol1, const char *protocol2)
    : Module_Worker_1(protocol1, protocol2) {}

int Worker1::process(Logger& log) {
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
        log.error("Failed to parse incoming ISMRMRD Header");
      }

      log.info("I am worker 1");

      //*** Message body
      receiver.recv(&body_msg);
      auto acq = static_cast<complex_float_t *>(body_msg.data());

      std::stringstream ss;
      ss << "Data:: " << real(acq[4]) << " " << imag(acq[4]);
      log.info(ss.str());

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
