
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "zhelpers.hpp"

int main()
{

    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5556");
    publisher.bind("ipc://message.ipc");

    while (1)
    {

        s_sendmore(publisher, "music");
        s_send(publisher, "<Brand New Day>");
        s_sendmore(publisher, "music");
        s_send(publisher, "<Rain>");
        s_sendmore(publisher, "programing");
        s_send(publisher, "C++");
        s_sendmore(publisher, "programing");
        s_send(publisher, "Java");
        s_sendmore(publisher, "programing");
        s_send(publisher, "Python");

        sleep(1);
    }
    return 0;
}
