#include "worker2.h"
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <spdlog/spdlog.h>
#include "zhelpers.hpp"

namespace FleXRP{

Worker2::Worker2(const char *protocol1, const char *protocol2):
    Module_Worker_2(protocol1, protocol2)
{}

int Worker2::process()
{
    //  Process messages from both sockets
    while (true) {
        zmq::poll (&items [0], 2, -1);

        if (items [0].revents & ZMQ_POLLIN) {
            receiver.recv(&message);

            // Deserialize the header
            ISMRMRD::IsmrmrdHeader h;
            try
            {
                ISMRMRD::deserialize( static_cast<char *>(message.data()), h );
            }
            catch (...)
            {
                spdlog::error("Failed to parse incoming ISMRMRD Header");
            }

            spdlog::info("I am worker 2");

            spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name, h.userParameters->userParameterLong[0].value);


            //*** Message body
            receiver.recv(&body_msg);
            auto acq = static_cast<complex_float_t*>(body_msg.data());

            spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));


            //  Do the work
            s_sleep(1000);

            sender.send(message);
            sender.send(body_msg);
        }
        //  Any waiting controller command acts as 'KILL'
        if (items [1].revents & ZMQ_POLLIN) {
            break;
        }
    }

    return EXIT_SUCCESS;
}

}

