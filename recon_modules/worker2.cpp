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

#include <string>

int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        spdlog::error("Usage: {} input_port output_port", argv[0]);
        return EXIT_FAILURE;
    }

    zmq::context_t context(1);
    
    //  Socket to receive messages on
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.bind(argv[1]);

    //  Socket to send messages to
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.bind(argv[2]);

    //  Socket for control input
    zmq::socket_t controller (context, ZMQ_SUB);
    controller.connect("tcp://localhost:7777");
    controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    //  Process messages from receiver and controller
    zmq::pollitem_t items [] = {
        { static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0 },
        { static_cast<void *>(controller), 0, ZMQ_POLLIN, 0 }
    };
    //  Process messages from both sockets
    while (true) {
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

            spdlog::info("I am worker 2");

            spdlog::info("{} is {}",h.userParameters->userParameterLong[0].name, h.userParameters->userParameterLong[0].value);


            //*** Message body
            receiver.recv(&body_msg);
            auto acq = static_cast<complex_float_t*>(body_msg.data());

            spdlog::info("Data:: {} {}", real(acq[4]), imag(acq[4]));

            s_sleep(5000);

            sender.send(message);
            sender.send(body_msg);

        }
        //  Any waiting controller command acts as 'KILL'
        if (items [1].revents & ZMQ_POLLIN) {
            break;
        }
    }
    
    //  Finished

    return 0;
}
