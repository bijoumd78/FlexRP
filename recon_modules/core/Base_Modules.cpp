#include "Base_Modules.h"
#include <logger.h>
#if defined(_WIN32)
#include <chrono>
#endif
#include "zhelpers.hpp"

namespace FlexRP {

#if defined(_WIN32)
int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
   namespace sc                 = std::chrono;
   sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
   sc::seconds s                = sc::duration_cast<sc::seconds>(d);
   tp->tv_sec                   = s.count();
   tp->tv_usec                  = sc::duration_cast<sc::microseconds>(d - s).count();

   return 0;
}
#endif // _WIN32

Module_Worker_1::Module_Worker_1(const char *protocol1, const char *protocol2)
    : context(1),
      receiver(context, ZMQ_PULL),
      sender(context, ZMQ_PUSH),
      controller(context, ZMQ_SUB),
      items{{static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0},
            {static_cast<void *>(controller), 0, ZMQ_POLLIN, 0}} {
  receiver.connect(protocol1);
  sender.connect(protocol2);
  controller.connect("tcp://localhost:7777");
  controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

int Module_Worker_1::Init() { return EXIT_SUCCESS; }

int Module_Worker_1::prepare() { return EXIT_SUCCESS; }

int Module_Worker_1::finalize() { return EXIT_SUCCESS; }

Module_Worker_2::Module_Worker_2(const char *protocol1, const char *protocol2)
    : context(1),
      receiver(context, ZMQ_PULL),
      sender(context, ZMQ_PUSH),
      controller(context, ZMQ_SUB),
      items{{static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0},
            {static_cast<void *>(controller), 0, ZMQ_POLLIN, 0}} {
  receiver.bind(protocol1);
  sender.bind(protocol2);
  controller.connect("tcp://localhost:7777");
  controller.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

int Module_Worker_2::Init() { return EXIT_SUCCESS; }

int Module_Worker_2::prepare() { return EXIT_SUCCESS; }

int Module_Worker_2::finalize() { return EXIT_SUCCESS; }

Module_Sink_1::Module_Sink_1(const char *protocol)
    : context(1),
      receiver(context, ZMQ_PULL),
      sender_cl(context, ZMQ_PUSH),
      controller(context, ZMQ_PUB),
      timer(context, ZMQ_PULL),
      items{{static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0},
            {static_cast<void *>(controller), 0, ZMQ_POLLIN, 0}},
      tstart() {
  receiver.bind(protocol);
  sender_cl.connect("tcp://localhost:6666");
  controller.bind("tcp://*:7777");
  timer.bind("tcp://*:8888");
  // Wait for start of batch
  (void)s_recv(timer);
  // Start our clock now
  gettimeofday(&tstart, nullptr);
}

Module_Sink_1::~Module_Sink_1() {
  // Calculate and report duration of batch
  struct timeval tend {
  }, tdiff{};
  gettimeofday(&tend, nullptr);

  if (tend.tv_usec < tstart.tv_usec) {
    tdiff.tv_sec = tend.tv_sec - tstart.tv_sec - 1;
    tdiff.tv_usec = 1000000 + tend.tv_usec - tstart.tv_usec;
  } else {
    tdiff.tv_sec = tend.tv_sec - tstart.tv_sec;
    tdiff.tv_usec = tend.tv_usec - tstart.tv_usec;
  }

  auto total_msec =
      static_cast<int>(tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000);

  try{
  Logger::info("Total elapsed time: {} msec", total_msec);
  }
  catch (const std::exception& e)
  {
      std::cout << e.what() << std::endl;
  }

  // Send kill signal to workers
  s_send(controller, "KILL");
}

int Module_Sink_1::Init() { return EXIT_SUCCESS; }

int Module_Sink_1::prepare() { return EXIT_SUCCESS; }

int Module_Sink_1::finalize() { return EXIT_SUCCESS; }

Module_Sink_2::Module_Sink_2(const char *protocol)
    : context(1),
      receiver(context, ZMQ_PULL),
      sender_cl(context, ZMQ_PUSH),
      controller(context, ZMQ_PUB),
      timer(context, ZMQ_PULL),
      items{{static_cast<void *>(receiver), 0, ZMQ_POLLIN, 0},
            {static_cast<void *>(controller), 0, ZMQ_POLLIN, 0}},
      tstart() {
  receiver.connect(protocol);
  sender_cl.connect("tcp://localhost:6666");
  controller.bind("tcp://*:7777");
  timer.bind("tcp://*:8888");
  // Wait for start of batch
  (void)s_recv(timer);
  // Start our clock now
  gettimeofday(&tstart, nullptr);
}

Module_Sink_2::~Module_Sink_2() {
  // Calculate and report duration of batch
  struct timeval tend {
  }, tdiff{};
  gettimeofday(&tend, nullptr);

  if (tend.tv_usec < tstart.tv_usec) {
    tdiff.tv_sec = tend.tv_sec - tstart.tv_sec - 1;
    tdiff.tv_usec = 1000000 + tend.tv_usec - tstart.tv_usec;
  } else {
    tdiff.tv_sec = tend.tv_sec - tstart.tv_sec;
    tdiff.tv_usec = tend.tv_usec - tstart.tv_usec;
  }

  auto total_msec =
      static_cast<int>(tdiff.tv_sec * 1000 + tdiff.tv_usec / 1000);
  Logger::info("Total elapsed time: {} msec", total_msec);

  // Send kill signal to workers
  s_send(controller, "KILL");
}

int Module_Sink_2::Init() { return EXIT_SUCCESS; }

int Module_Sink_2::prepare() { return EXIT_SUCCESS; }

int Module_Sink_2::finalize() { return EXIT_SUCCESS; }

}  // namespace FleXRP
