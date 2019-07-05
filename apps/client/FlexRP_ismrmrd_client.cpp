/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include <zmq.hpp>

#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>


int main (int argc, char* argv[])
{

    if(argc < 2){
        std::cout << "Usage: send_req input_file_name\n";
        return -1;
    }

    zmq::context_t context (1);

    //  Socket to send messages on
    zmq::socket_t  sender(context, ZMQ_PUSH);
    sender.bind("tcp://*:5555");

    std::cout << "Press Enter when the workers are ready: " << std::endl;
    getchar ();
    std::cout << "Sending tasks to workers...\n" << std::endl;

    //  The first message is "0" and signals start of batch
    zmq::socket_t sink(context, ZMQ_PUSH);
    sink.connect("tcp://localhost:5558");
    zmq::message_t message(2);
    memcpy(message.data(), "0", 1);
    sink.send(message);


    // Load ISMRMD file
    ISMRMRD::ISMRMRD_Acquisition acq;
    ISMRMRD::ISMRMRD_Dataset dataset;
    uint32_t index;
    uint32_t nacq_read;
    const char *filename = argv[1];
    const char *groupname = "/dataset";
    

    ismrmrd_init_dataset(&dataset, filename, groupname);
    ismrmrd_open_dataset(&dataset, false);

    /* Read the header */
    char *xmlstring = ismrmrd_read_header(&dataset);
    message.rebuild(strlen(xmlstring));
    memcpy(message.data(), xmlstring, strlen(xmlstring));
    sender.send(message);
    // Cleanup and close the stream
    free(xmlstring);


    /** Get the raw data **/
    /* Get the number of acquisitions */
    nacq_read = ismrmrd_get_number_of_acquisitions(&dataset);
    /* read the next to last one */
    ismrmrd_init_acquisition(&acq);
    index = 0;
    if (nacq_read>1) {
        index = nacq_read - 1;
    }
    else {
        index = 0;
    }

    ismrmrd_read_acquisition(&dataset, index, &acq);

    std::cout << "Data:: " << real(acq.data[4]) << " " << imag(acq.data[4]) << std::endl;

    zmq::message_t body_msg(2*index*sizeof(float));
    std::memcpy(body_msg.data(), acq.data, 2*index*sizeof(float));
    sender.send(body_msg);


    //ismrmrd_cleanup_acquisition(&acq);
    ismrmrd_close_dataset(&dataset);

    return 0;
}
