/****************************************************************************
 * Description: New_Recon_Framework
 * Author: Mahamadou Diakite
 * Lang: C/C++
 *  Date: 3/15/2018
 * Version: 0.0.1
 *****************************************************************************/

#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <spdlog/spdlog.h>
#if !defined (WIN32)
#include <unistd.h>
#endif
#include <boost/asio/io_service.hpp>
#include <boost/program_options.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <zmq.hpp>
#include "flexrp_client.h"

int main(int argc, char* argv[]) {
  namespace po = boost::program_options;
  std::string in_h5_filename{};
  std::string out_h5_filename{};
  std::string config_xml_filename{};

  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")(
        "filename,f", po::value<std::string>(&in_h5_filename), "Input file")(
        "outputfile,o",
        po::value<std::string>(&out_h5_filename)->default_value("out.h5"),
        "Output file")("config,c", po::value<std::string>(&config_xml_filename),
                       "FlexRP Configuration file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }

    if (vm.count("filename")) {
      spdlog::info("The input filename is {}",
                   vm["filename"].as<std::string>());
    } else {
      spdlog::error("h5 data file name is required!");
      std::cout << desc << std::endl;
      return EXIT_FAILURE;
    }

    if (vm.count("config")) {
      spdlog::info("The configuration file is {}",
                   vm["config"].as<std::string>());
    } else {
      spdlog::error("The configuration file is required!");
      std::cout << desc << std::endl;
      return EXIT_FAILURE;
    }
  } catch (const std::exception& e) {
    spdlog::error("error: {}", e.what());
    return EXIT_FAILURE;
  } catch (...) {
    spdlog::error("Exception of unknown type!");
  }

  // Send configuration file
  const std::string address{"localhost"};
  const std::string port{"8080"};
  const std::string filePath{config_xml_filename};

  try {
    boost::asio::io_service ioService;

    boost::asio::ip::tcp::resolver resolver(ioService);
    auto endpointIterator = resolver.resolve({address, port});
    FlexRP_client::Client client(ioService, endpointIterator, filePath);

    ioService.run();
  } catch (const std::fstream::failure& e) {
    spdlog::error(e.what());
  } catch (const std::exception& e) {
    spdlog::error("Exception: {}", e.what());
  }

  // Program starts here
  zmq::context_t context(1);

  spdlog::info("Sending tasks to workers...");

  //  The first message is "0" and signals start of batch
  zmq::socket_t sink(context, ZMQ_PUSH);
  sink.connect("tcp://localhost:8888");
  zmq::message_t message(2);
  memcpy(message.data(), "0", 1);
  sink.send(message);

  // Load ISMRMD file
  ISMRMRD::ISMRMRD_Acquisition acq;
  ISMRMRD::ISMRMRD_Dataset dataset;
  uint32_t index;
  uint32_t nacq_read;
  const char* groupname = "/dataset";

  ismrmrd_init_dataset(&dataset, in_h5_filename.c_str(), groupname);
  ismrmrd_open_dataset(&dataset, false);

  //  Socket to send messages on
  zmq::socket_t sender(context, ZMQ_PUSH);
  sender.bind("tcp://*:5555");

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
  index = nacq_read > 1 ? nacq_read - 1 : 0;

  ismrmrd_read_acquisition(&dataset, index, &acq);

  spdlog::info("Data:: {} {}", real(acq.data[4]), imag(acq.data[4]));

  zmq::message_t body_msg(2 * index * sizeof(float));
  std::memcpy(body_msg.data(), acq.data, 2 * index * sizeof(float));
  sender.send(body_msg);
  ismrmrd_close_dataset(&dataset);

  //  Socket to receive message from server
  zmq::socket_t receiver_sr(context, ZMQ_PULL);
  receiver_sr.bind("tcp://*:6666");
  zmq::pollitem_t items[] = {
      {static_cast<void*>(receiver_sr), 0, ZMQ_POLLIN, 0}};

  zmq::poll(&items[0], 1, -1);
  if (items[0].revents & ZMQ_POLLIN) {
    receiver_sr.recv(&message);
    // Deserialize the header
    ISMRMRD::IsmrmrdHeader h;
    try {
      ISMRMRD::deserialize(static_cast<char*>(message.data()), h);
    } catch (...) {
      spdlog::error("Failed to parse incoming ISMRMRD Header");
    }

    spdlog::info("Received data from server...");

    /* spdlog::info("{} is {}", h.userParameters->userParameterLong[0].name,
                 h.userParameters->userParameterLong[0].value);*/

    //*** Message body
    receiver_sr.recv(&body_msg);
    const auto acq_r = static_cast<complex_float_t*>(body_msg.data());

    spdlog::info("Data:: {} {}", real(acq_r[4]), imag(acq_r[4]));

    // Write data to file
    ISMRMRD::ISMRMRD_NDArray arr;
    ISMRMRD::ISMRMRD_Dataset dataset2;

    ismrmrd_init_dataset(&dataset2, out_h5_filename.c_str(), groupname);
    ismrmrd_open_dataset(&dataset2, true);

    // Write the XML header
    ismrmrd_write_header(&dataset2, static_cast<char*>(message.data()));

    ismrmrd_init_ndarray(&arr);
    arr.data_type = ISMRMRD::ISMRMRD_CXFLOAT;
    arr.ndim = 1;
    arr.dims[0] = index;
    ismrmrd_make_consistent_ndarray(&arr);

    memcpy(arr.data, acq_r, 2 * index * sizeof(float));

    // Write the data blob
    ismrmrd_append_array(&dataset2, groupname, &arr);

    // Close the dataset
    ismrmrd_close_dataset(&dataset2);
  }

  return 0;
}
