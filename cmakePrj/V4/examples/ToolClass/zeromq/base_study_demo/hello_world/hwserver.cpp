//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include "zhelpers.hpp"

int main()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    int count=0;
    while (true)
    {
        std::string recvStr;
        recvStr = s_recv(socket);
        std::cout << "server Received " << recvStr << " " << count << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        std::string sendStr = "World";
        s_send(socket, sendStr);
        std::cout << "server Send " << sendStr << " " << count << std::endl;
        count++;
    }
    return 0;
}
