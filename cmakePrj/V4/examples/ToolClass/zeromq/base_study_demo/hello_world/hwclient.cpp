//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "zhelpers.hpp"

int main()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        std::string sendStr = "Hello";
        s_send(socket, sendStr);
        std::cout << "client Send " << sendStr << " " << request_nbr << std::endl;

        //  Get the reply.
        std::string recvStr;
        recvStr = s_recv(socket);
        std::cout << "client Received " << recvStr << " " << request_nbr << std::endl;
    }
    return 0;
}
