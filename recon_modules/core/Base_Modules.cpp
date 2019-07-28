#include "Base_Modules.h"
#include <spdlog/spdlog.h>
#include "zhelpers.hpp"


namespace FleXRP{

Module_Worker_1::Module_Worker_1(const char *protocol1, const char *protocol2) :
    context   ( 1                 ),
    receiver  ( context, ZMQ_PULL ),
    sender    ( context, ZMQ_PUSH ),
    controller( context, ZMQ_SUB  ),
    message   (                   ),
    body_msg  (                   )
{
    receiver.connect(protocol1);
    sender.connect(protocol2);
    controller.connect("tcp://localhost:7777");
    controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    items [0] = { static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0   };
    items [1] = { static_cast<void *>(controller), 0, ZMQ_POLLIN, 0 };
}

int Module_Worker_1::Init()
{
    return EXIT_SUCCESS;
}

int Module_Worker_1::prepare()
{
    return EXIT_SUCCESS;
}

int Module_Worker_1::finalize()
{
    return EXIT_SUCCESS;
}

Module_Worker_2::Module_Worker_2(const char *protocol1, const char *protocol2) :
    context   ( 1                 ),
    receiver  ( context, ZMQ_PULL ),
    sender    ( context, ZMQ_PUSH ),
    controller( context, ZMQ_SUB  ),
    message   (                   ),
    body_msg  (                   )
{
    receiver.bind(protocol1);
    sender.bind(protocol2);
    controller.connect("tcp://localhost:7777");
    controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    items [0] = { static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0   };
    items [1] = { static_cast<void *>(controller), 0, ZMQ_POLLIN, 0 };
}

int Module_Worker_2::Init()
{
    return EXIT_SUCCESS;
}

int Module_Worker_2::prepare()
{
    return EXIT_SUCCESS;
}

int Module_Worker_2::finalize()
{
    return EXIT_SUCCESS;
}

Module_Sink::Module_Sink(const char *protocol):
    context   ( 1                 ),
    receiver  ( context, ZMQ_PULL ),
    controller( context, ZMQ_PUB  ),
    message   (                   ),
    body_msg  (                   ),
    tstart    (                   )
{
    receiver.bind(protocol);
    controller.bind("tcp://*:7777");
    // Wait for start of batch
    s_recv (receiver);
    // Start our clock now
    gettimeofday (&tstart, nullptr);
    // Process messages from receiver and controller
    items [0] = { static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0   };
    items [1] = { static_cast<void *>(controller), 0, ZMQ_POLLIN, 0 };
}

Module_Sink::~Module_Sink()
{
    // Calculate and report duration of batch
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

    // Send kill signal to workers
    s_send (controller, "KILL");
}

int Module_Sink::Init()
{
    return EXIT_SUCCESS;
}

int Module_Sink::prepare()
{
    return EXIT_SUCCESS;
}

int Module_Sink::finalize()
{
    return EXIT_SUCCESS;
}

} // End of FleXR
