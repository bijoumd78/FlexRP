#ifndef BASE_MODULES_H
#define BASE_MODULES_H

#include <zmq.hpp>

namespace FleXRP{

class Module_Worker_1 {

public:
    Module_Worker_1(const char *protocol1, const char *protocol2);
    int process();

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
    int process();

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
class Module_Sink{

public:
    Module_Sink(const char *protocol);
    int process();
    ~Module_Sink();

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
