//
//  持久化的订阅者
//

#include "zhelpers.hpp"

int main()
{
    //  Prepare our context and subscriber
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    subscriber.setsockopt(ZMQ_IDENTITY, "Hello", 5);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.connect("tcp://localhost:5565");

    zmq::socket_t sync(context, ZMQ_PUSH);
    sync.connect("tcp://localhost:5564");
    s_send(sync, "");

    while (1)
    {
        std::string str = s_recv(subscriber);

        std::cout << str << std::endl;
        if (str == "END")
        {
            break;
        }
    }
    return 0;
}