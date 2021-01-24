/*
    Multithreaded Hello World server in C
*/

#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>
#include "zhelpers.hpp"

void *worker_routine(void *arg)
{
    zmq::context_t *context = (zmq::context_t *)arg;

    zmq::socket_t socket(*context, ZMQ_REP);
    socket.connect("inproc://workers");

    while (true)
    { 
        //  Wait for next request from client
        std::string recvStr;
        recvStr = s_recv(socket);
        std::cout << "server pthread_id: "<<pthread_self()<<" Received request: [" << recvStr << "]" << std::endl;

        //  Do some 'work'
        sleep(1);

        //  Send reply back to client
        std::string sendStr = "server pthread_id: "+ std::to_string(pthread_self())+" send [I'm server Hello.]";
        std::cout << sendStr << std::endl;
        s_send(socket, sendStr);
    }
    return (NULL);
}

int main()
{
    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t clients(context, ZMQ_ROUTER);
    clients.bind("tcp://*:5555");
    zmq::socket_t workers(context, ZMQ_DEALER);
    workers.bind("inproc://workers");

    //  Launch pool of worker threads
    for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_routine, (void *)&context);
    }
    //  Connect work threads to client threads via a queue
    zmq_device(ZMQ_QUEUE, clients, workers);
    return 0;
}
