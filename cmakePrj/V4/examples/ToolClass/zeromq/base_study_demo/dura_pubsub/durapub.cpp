//
//  发布者 - 连接持久化的订阅者
//
#include "zhelpers.hpp"

int main () {
    
    zmq::context_t context(1);

    zmq::socket_t sync(context, ZMQ_PULL);
    sync.bind("tcp://*:5564");

    zmq::socket_t publisher(context, ZMQ_PUB);
    
    //  避免慢持久化订阅者消息溢出的问题,还未实现
    //int hwm = 5;
    //publisher.setsockopt(ZMQ_SNDHWM, &hwm, sizeof (hwm));

    publisher.bind("tcp://*:5565");

    s_recv(sync);

    int update_nbr;
    for (update_nbr = 0; update_nbr < 10; update_nbr++) {
        char string [20];
        sprintf (string, "Update %d", update_nbr);
        s_send (publisher, string);
        sleep (1);
    }

    s_send (publisher, "END");
    return 0;
}
