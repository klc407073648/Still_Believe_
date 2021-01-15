//
//  偏执海盗队列
//
#include "zmsg.hpp"

#include <stdint.h>
#include <vector>

#define HEARTBEAT_LIVENESS 3    //  心跳健康度，3-5是合理的
#define HEARTBEAT_INTERVAL 1000 //  单位：毫秒

//  使用以下结构表示worker队列中的一个有效的worker

typedef struct
{
    std::string identity; // worker地址
    int64_t expiry; // 过期时间
} worker_t;

//  Insert worker at end of queue, reset expiry
//  Worker must not already be in queue
static void
s_worker_append(std::vector<worker_t> &queue, std::string &identity)
{
    bool found = false;
    for (std::vector<worker_t>::iterator it = queue.begin(); it < queue.end(); it++)
    {
        if (it->identity.compare(identity) == 0)
        {
            std::cout << "E: duplicate worker identity " << identity.c_str() << std::endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        worker_t worker;
        worker.identity = identity;
        worker.expiry = s_clock() + HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS;
        queue.push_back(worker);
    }
}

//  Remove worker from queue, if present
static void
s_worker_delete(std::vector<worker_t> &queue, std::string &identity)
{
    for (std::vector<worker_t>::iterator it = queue.begin(); it < queue.end(); it++)
    {
        if (it->identity.compare(identity) == 0)
        {
            it = queue.erase(it);
            break;
        }
    }
}

//  Reset worker expiry, worker must be present
static void
s_worker_refresh(std::vector<worker_t> &queue, std::string &identity)
{
    bool found = false;
    for (std::vector<worker_t>::iterator it = queue.begin(); it < queue.end(); it++)
    {
        if (it->identity.compare(identity) == 0)
        {
            it->expiry = s_clock() + HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS;
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::cout << "E: worker " << identity << " not ready" << std::endl;
    }
}

//  Pop next available worker off queue, return identity
static std::string
s_worker_dequeue(std::vector<worker_t> &queue)
{
    assert(queue.size());
    std::string identity = queue[0].identity;
    queue.erase(queue.begin());
    return identity;
}

//  Look for & kill expired workers
static void
s_queue_purge(std::vector<worker_t> &queue)
{
    int64_t clock = s_clock();
    for (std::vector<worker_t>::iterator it = queue.begin(); it < queue.end(); it++)
    {
        if (clock > it->expiry)
        {
            it = queue.erase(it) - 1;
        }
    }
}

int main(void)
{
    s_version_assert(2, 1);

    //  Prepare our context and sockets
    zmq::context_t context(1);
    zmq::socket_t frontend(context, ZMQ_ROUTER);
    zmq::socket_t backend(context, ZMQ_ROUTER);
    frontend.bind("tcp://*:5555"); //  For clients
    backend.bind("tcp://*:5556");  //  For workers

    //  Queue of available workers
    std::vector<worker_t> queue;

    //  规律地发送心跳
    int64_t heartbeat_at = s_clock() + HEARTBEAT_INTERVAL;

    while (1)
    {
        zmq::pollitem_t items[] = {
            {backend, 0, ZMQ_POLLIN, 0},
            {frontend, 0, ZMQ_POLLIN, 0}};

        //  当存在可用worker时轮询前端端点
        if (queue.size())
        {
            zmq::poll(items, 2, HEARTBEAT_INTERVAL * 1);
        }
        else
        {
            zmq::poll(items, 1, HEARTBEAT_INTERVAL * 1);
        }

        //  处理后端worker请求
        if (items[0].revents & ZMQ_POLLIN)
        {
            zmsg msg(backend);
            std::string identity(msg.unwrap());

            //  Return reply to client if it's not a control message
            //  处理控制消息，
            if (msg.parts() == 1)
            {
                if (strcmp(msg.address(), "READY") == 0)
                {
                    s_worker_delete(queue, identity);
                    s_worker_append(queue, identity);
                }
                else
                {
                    if (strcmp(msg.address(), "HEARTBEAT") == 0)
                    {
                        s_worker_refresh(queue, identity);
                    }
                    else
                    {
                        std::cout << "E: invalid message from " << identity << std::endl;
                        msg.dump();
                    }
                }
            }
            else //  将应答转发给client
            {
                msg.send(frontend);
                s_worker_append(queue, identity);
            }
        }
        if (items[1].revents & ZMQ_POLLIN)
        {
            //  Now get next client request, route to next worker
            zmsg msg(frontend);
            std::string identity = std::string(s_worker_dequeue(queue));
            msg.push_front((char *)identity.c_str());
            msg.send(backend);
        }

        //  Send heartbeats to idle workers if it's time
        if (s_clock() > heartbeat_at)
        {
            for (std::vector<worker_t>::iterator it = queue.begin(); it < queue.end(); it++)
            {
                zmsg msg("HEARTBEAT");
                msg.wrap(it->identity.c_str(), NULL);
                msg.send(backend);
            }
            heartbeat_at = s_clock() + HEARTBEAT_INTERVAL;
        }
        s_queue_purge(queue);
    }
    //  We never exit the main loop
    //  But pretend to do the right shutdown anyhow
    queue.clear();
    return 0;
}
