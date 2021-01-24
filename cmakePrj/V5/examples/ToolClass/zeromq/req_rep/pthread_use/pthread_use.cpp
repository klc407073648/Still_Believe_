//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

int var = 100;

void *tfn(void *arg)
{
    int i;
    i = (int)arg;
    
    sleep(i);
    if (i == 0) {
        zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_REP);
        socket.bind ("tcp://*:5555");

        int recvConut=0;
        while (true) {
            zmq::message_t request;

            //  Wait for next request from client
            socket.recv (&request);
            std::cout << "server Received "<<request<<" "<< std::endl;

            //  Do some 'work'
            sleep (1);

            //  Send reply back to client
            zmq::message_t reply (5);
            memcpy ((void *) reply.data (), "World", 5);
            socket.send (reply);

            recvConut++;
            if(recvConut>=10)
            {
                break;
            }

        }
        var = 111;
        printf("I'm %dth pthread, pthread_id = %lu\n var = %d\n", i, pthread_self(), var);
        pthread_exit((void *)var);

    } else  if (i == 1) {
        
        zmq::context_t context (1);
        zmq::socket_t socket (context, ZMQ_REQ);

        std::cout << "Connecting to hello world server..." << std::endl;
        socket.connect ("tcp://localhost:5555");

        //  Do 10 requests, waiting each time for a response
        for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
            zmq::message_t request (6);
            memcpy ((void *) request.data (), "Hello", 5);
            std::cout << "client Send "<<request<<" "<<request_nbr << "..." << std::endl;
            socket.send (request);

            //  Get the reply.
            zmq::message_t reply;
            socket.recv (&reply);
            std::cout << "client Received "<<reply<<" "<<request_nbr << std::endl;
        }
        var = 222;
        printf("I'm %dth pthread, pthread_id = %lu\n var = %d\n", i, pthread_self(), var);
        pthread_exit((void *)var);

    } else  {
        printf("I'm %dth pthread, pthread_id = %lu\n var = %d\n", i, pthread_self(), var);
        pthread_exit((void *)var);
    }

    return NULL;
}

int main () {

    pthread_t tid[5];
    int i;
    int *ret[5];  

    for (i = 0; i < 5; i++)
        pthread_create(&tid[i], NULL, tfn, (void *)i);

    for (i = 0; i < 5; i++) {
        pthread_join(tid[i], (void **)&ret[i]);
        printf("-------%d 's ret = %d\n", i, (int)ret[i]);
    }

    printf("I'm main pthread tid = %lu\t var = %d\n", pthread_self(), var);

    sleep(i);

    return 0;
}
