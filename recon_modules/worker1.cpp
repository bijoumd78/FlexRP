/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include "zhelpers.hpp"

#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>

#include <string>


int main (int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: worker1 input_port output_port\n";
        return EXIT_FAILURE;
    }

    zmq::context_t context(1);
    
    //  Socket to receive messages on
    zmq::socket_t receiver(context, ZMQ_PULL);
    receiver.connect(argv[1]);

    //  Socket to send messages to
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.connect(argv[2]);

    //  Socket for control input
    zmq::socket_t controller (context, ZMQ_SUB);
    controller.connect("tcp://localhost:5559");
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
                std::cerr << "Failed to parse incoming ISMRMRD Header\n";
            }

            std::cout<< "\n\n" <<"I am worker 1 \n";

            std::cout << h.userParameters->userParameterLong[0].name << " is " << h.userParameters->userParameterLong[0].value << std::endl;


            //*** Message body
            receiver.recv(&body_msg);
            auto acq = static_cast<complex_float_t*>(body_msg.data());

            std::cout << "Data:: " << real(acq[4]) << " " << imag(acq[4]) << std::endl;


            //  Do the work
            s_sleep(5000);


            sender.send(message);
            sender.send(body_msg);

        }
        //  Any waiting controller command acts as 'KILL'
        if (items [1].revents & ZMQ_POLLIN) {
            std::cout << std::endl;
            break;                      //  Exit loop
        }
    }
    //  Finished

    return 0;
}
