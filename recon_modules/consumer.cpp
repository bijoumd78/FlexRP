/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include "zhelpers.hpp"
#include <spdlog/spdlog.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>


int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        spdlog::error("Usage: producer input_port");
        return EXIT_FAILURE;
    }

    zmq::context_t context(1);

    //  Socket to receive messages on
    zmq::socket_t receiver (context, ZMQ_PULL);
    receiver.bind(argv[1]);

    //  Socket for worker control
    zmq::socket_t controller (context, ZMQ_PUB);
    controller.bind("tcp://*:7777");

    //  Wait for start of batch
    s_recv (receiver);

    //  Start our clock now
    struct timeval tstart;
    gettimeofday (&tstart, nullptr);

    
    //  Process messages from receiver and controller
    zmq::pollitem_t items [] = {
        { static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0 },
        { static_cast<void *>(controller), 0, ZMQ_POLLIN, 0 }
    };

    //  Process 100 confirmations
    //int task_nbr;
    //for (task_nbr = 0; task_nbr < 1; task_nbr++) {


    zmq::message_t message, body_msg;
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

        spdlog::info("I am the last worker in the chain...");

        spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name, h.userParameters->userParameterLong[0].value);


        //*** Message body
        receiver.recv(&body_msg);
        auto acq = static_cast<complex_float_t*>(body_msg.data());

        spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));
        //  }
    }

    //  Calculate and report duration of batch
    struct timeval tend, tdiff;
    gettimeofday (&tend, nullptr);

    if (tend.tv_usec < tstart.tv_usec) {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec - 1;
        tdiff.tv_usec = 1000000 + tend.tv_usec - tstart.tv_usec;
    }
    else {
        tdiff.tv_sec = tend.tv_sec - tstart.tv_sec;
        tdiff.tv_usec = tend.tv_usec - tstart.tv_usec;
    }

    auto total_msec = static_cast<int>(tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000);
    spdlog::info("Total elapsed time: {} msec",  total_msec);

    //  Send kill signal to workers
    s_send (controller, "KILL");

    return 0;
}
