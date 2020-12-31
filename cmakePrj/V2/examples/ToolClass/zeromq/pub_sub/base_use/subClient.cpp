#include <zmq.hpp>
#include <iostream>
#include <sstream>
#include "zhelpers.hpp"

int main(int argc, char *argv[])
{
    zmq::context_t context(1);

    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");

    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = (argc > 1) ? argv[1] : "music";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));

    while (1)
    {

        std::string topic = s_recv(subscriber);

        std::string content = s_recv(subscriber);

        std::cout << "[" << topic << "] " << content << std::endl;
    }
    return 0;
}
