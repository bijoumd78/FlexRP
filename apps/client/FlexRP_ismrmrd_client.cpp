/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include <zmq.hpp>
#include <boost/program_options.hpp>
#include <boost/asio/io_service.hpp>
#include <ismrmrd/ismrmrd.h>
#include "flexrp_client.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <exception>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <cstring>


int main (int argc, char* argv[])
{
    namespace po = boost::program_options;
    std::string in_h5_filename{};
    std::string config_xml_filename{};

    try{
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("filename,f", po::value<std::string>(&in_h5_filename), "Input file")
                ("config,c", po::value<std::string>(&config_xml_filename), "FlexRP Configuration file (remote)")
                ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc <<std::endl;
            return 0;
        }

        if (vm.count("filename")) {
            spdlog::info("The input filename is {}", vm["filename"].as<std::string>());
        } else {
            spdlog::error( "h5 data file name is required!");
            std::cout << desc <<std::endl;
            return EXIT_FAILURE;
        }

        if (vm.count("config")) {
            spdlog::info("The configuration file is {}", vm["config"].as<std::string>());
        } else {
            spdlog::error( "The configuration file is required!");
            std::cout << desc <<std::endl;
            return EXIT_FAILURE;
        }
    }
    catch(std::exception& e) {
        spdlog::error( "error: {}", e.what());
        return EXIT_FAILURE;
    }
    catch(...) {
        spdlog::error( "Exception of unknown type!" );
    }

    // Send configuration file
    const std::string address{"localhost"};
    const std::string port{"8080"};
    const std::string filePath {config_xml_filename};

    try {
        boost::asio::io_service ioService;

        boost::asio::ip::tcp::resolver resolver(ioService);
        auto endpointIterator = resolver.resolve({ address, port });
        FlexRP_client::Client client(ioService, endpointIterator, filePath);

        ioService.run();
    } catch (std::fstream::failure& e) {
        spdlog::error(e.what());
    } catch (std::exception& e) {
        spdlog::error("Exception: {}", e.what());
    }

    // Program starts here
    zmq::context_t context (1);

    //  Socket to send messages on
    zmq::socket_t  sender(context, ZMQ_PUSH);
    sender.bind("tcp://*:5555");

    spdlog::info("Press Enter when the workers are ready!");
    getchar ();
    spdlog::info("Sending tasks to workers...");

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
    const char *groupname = "/dataset";

    ismrmrd_init_dataset(&dataset, in_h5_filename.c_str(), groupname);
    ismrmrd_open_dataset(&dataset, false);

    // Read the header
    auto xmlstring = ismrmrd_read_header(&dataset);
    message.rebuild(strlen(xmlstring));
    memcpy(message.data(), xmlstring, strlen(xmlstring));
    sender.send(message);
    // Cleanup and close the stream
    free(xmlstring);

    // Get the raw data
    // Get the number of acquisitions
    nacq_read = ismrmrd_get_number_of_acquisitions(&dataset);
    // read the next to last one
    ismrmrd_init_acquisition(&acq);
    index = nacq_read>1 ? nacq_read - 1 : 0;

    ismrmrd_read_acquisition(&dataset, index, &acq);

    spdlog::info("Data:: {} {}", real(acq.data[4]), imag(acq.data[4]));

    zmq::message_t body_msg(2*index*sizeof(float));
    std::memcpy(body_msg.data(), acq.data, 2*index*sizeof(float));
    sender.send(body_msg);

    ismrmrd_close_dataset(&dataset);

    return 0;
}
