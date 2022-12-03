#include "worker2.h"
#include "flexrpsharedmemory.h"
#include "zhelpers.hpp"
#include "ThreadPool.h"
#include <ismrmrd/dataset.h>
#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/xml.h>
#include <logger.h>
#include <sstream>

namespace FlexRP {

Worker2::Worker2(const char *protocol1, const char *protocol2)
    : Module_Worker_2(protocol1, protocol2) {}

int Worker2::process(Logger& log) {
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

            log.info("I am worker 2");

            //*** Message body
            receiver.recv(&body_msg);
            auto acq = static_cast<complex_float_t *>(body_msg.data());

            std::stringstream ss;
            ss << "Data: " << real(acq[4]) << " " << imag(acq[4]);
            log.info(ss.str());
            //Reset string stream
            ss.str("");
            ss.clear();

            // Get properties (name, value)
            constexpr size_t n = 4;
            std::vector<std::string> property;
            FlexRP::FlexRPSharedMemory::getReconmoduleProperty(property, n);
            for (size_t e = 0; e < n; e += 2)
            {
                ss << "Property(name, value)  " << property[e] << " " << property[e + 1];
                log.info(ss.str());
                //Reset string stream
                ss.str("");
                ss.clear();
            }

#if 0
            //Spin couple threads (Sample multithreaded application)
            auto n_threads = std::thread::hardware_concurrency();
            Logger::info("Total thread(s) available on this system is {}", n_threads);

            ThreadPool pool( n_threads - 1);
            std::vector< std::future<int> > results;

            for(int i = 0; i < 100; ++i) {
                results.emplace_back(
                            pool.enqueue([i] {
                    Logger::info("hello {}", i );
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    Logger::info("world {}", i );
                    return i*i;
                })
                            );
            }

            for(auto && result: results)
                Logger::info("{}", result.get());
#endif

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
