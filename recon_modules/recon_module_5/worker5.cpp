#include "worker5.h"
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <spdlog/spdlog.h>
#include "zhelpers.hpp"


namespace FleXRP {

Worker5::Worker5(const char *protocol):
    Module_Sink_2(protocol)
{}

int FleXRP::Worker5::process()
{
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

        spdlog::info("I am worker 3");

        spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name, h.userParameters->userParameterLong[0].value);


        //*** Message body
        receiver.recv(&body_msg);
        auto acq = static_cast<complex_float_t*>(body_msg.data());

        spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));

    }

    return EXIT_SUCCESS;
}

}