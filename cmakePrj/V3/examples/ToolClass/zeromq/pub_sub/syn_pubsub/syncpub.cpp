//
//  Synchronized publisher in C++
//
// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>

#include "zhelpers.hpp"

//  We wait for 10 subscribers
#define SUBSCRIBERS_EXPECTED 3

int main()
{
    zmq::context_t context(1);

    //  Socket to talk to clients
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5561");

    //  Socket to receive signals
    zmq::socket_t syncservice(context, ZMQ_REP);
    syncservice.bind("tcp://*:5562");

    //  Get synchronization from subscribers
    int subscribers = 0;
    while (subscribers < SUBSCRIBERS_EXPECTED)
    {

        //  - wait for synchronization request
        s_recv(syncservice);

        //  - send synchronization reply
        s_send(syncservice, "");

        subscribers++;
    }

    //  Now broadcast exactly 1M updates followed by END
    int update_nbr;
    for (update_nbr = 0; update_nbr < 10; update_nbr++)
    {
        s_send(publisher, "Rhubarb");
    }

    s_send(publisher, "END");

    std::cout << "Send " << update_nbr << " updates" << std::endl;

    sleep(1); //  Give 0MQ time to flush output
    return 0;
}
