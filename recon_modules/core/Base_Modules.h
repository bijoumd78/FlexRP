#ifndef BASE_MODULES_H
#define BASE_MODULES_H

#include <zmq.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

namespace FleXRP{

class Module_Worker_1 {

public:
    Module_Worker_1(const char *protocol1, const char *protocol2);
    virtual ~Module_Worker_1() = default;

    virtual int Init();
    virtual int prepare();
    virtual int process() = 0;
    virtual int finalize();

protected:
    zmq::context_t context;
    zmq::socket_t receiver;
    zmq::socket_t sender;
    zmq::socket_t controller;
    zmq::message_t message;
    zmq::message_t body_msg;
    zmq::pollitem_t items [2];
};

class Module_Worker_2 {

public:
    Module_Worker_2(const char *protocol1, const char *protocol2 );
    virtual ~Module_Worker_2() = default;

    virtual int Init();
    virtual int prepare();
    virtual int process() = 0;
    virtual int finalize();

protected:
    zmq::context_t context;
    zmq::socket_t receiver;
    zmq::socket_t sender;
    zmq::socket_t controller;
    zmq::message_t message;
    zmq::message_t body_msg;
    zmq::pollitem_t items [2];
};

// Collects completed works
class Module_Sink_1{

public:
    explicit Module_Sink_1(const char *protocol);
    virtual ~Module_Sink_1();

    virtual int Init();
    virtual int prepare();
    virtual int process() = 0;
    virtual int finalize();

protected:
    zmq::context_t context;
    zmq::socket_t receiver;
    zmq::socket_t controller;
    zmq::message_t message;
    zmq::message_t body_msg;
    zmq::pollitem_t items [2];
    struct timeval tstart;
};

class Module_Sink_2{

public:
    explicit Module_Sink_2(const char *protocol);
    virtual ~Module_Sink_2();

    virtual int Init();
    virtual int prepare();
    virtual int process() = 0;
    virtual int finalize();

protected:
    zmq::context_t context;
    zmq::socket_t receiver;
    zmq::socket_t controller;
    zmq::message_t message;
    zmq::message_t body_msg;
    zmq::pollitem_t items [2];
    struct timeval tstart;
};
}

#endif //BASE_MODULES_H
