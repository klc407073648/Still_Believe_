//
//  使用LRU算法的装置
//  client和worker处于不同的线程中
//
#include "zhelpers.hpp"

#include <queue>

//  使用REQ套接字实现基本的请求-应答模式
//  由于s_send()和s_recv()不能处理0MQ的二进制套接字标识，
//  所以这里会生成一个可打印的字符串标识。
//
static void *
client_thread(void *arg)
{
    zmq::context_t context(1);
    zmq::socket_t client(context, ZMQ_REQ);
    s_set_id(client); //  设置可打印的标识
    client.connect("ipc://frontend.ipc");

    //  发送请求并获取应答信息
    s_send(client, "HELLO");
    std::string reply = s_recv(client);
    std::cout << "Client: " << reply << std::endl;
    return (NULL);
}

//  worker使用REQ套接字实现LRU算法
//
static void *
worker_thread(void *arg)
{
    zmq::context_t context(1);
    zmq::socket_t worker(context, ZMQ_REQ);
    s_set_id(worker); //  设置可打印的标识
    worker.connect("ipc://backend.ipc");

    //  告诉代理worker已经准备好
    s_send(worker, "READY");

    while (1)
    {
        //  将消息中空帧之前的所有内容（信封）保存起来，
        //  本例中空帧之前只有一帧，但可以有更多。
        std::string address = s_recv(worker);
        {
            std::string empty = s_recv(worker);
            assert(empty.size() == 0);
        }

        //  获取请求，并发送回应
        std::string request = s_recv(worker);
        std::cout << "Worker: " << request << std::endl;

        s_sendmore(worker, address);
        s_sendmore(worker, "");
        s_send(worker, "OK");
    }
    return (NULL);
}

int main(int argc, char *argv[])
{

    //  准备0MQ上下文和套接字
    zmq::context_t context(1);
    zmq::socket_t frontend(context, ZMQ_ROUTER);
    zmq::socket_t backend(context, ZMQ_ROUTER);
    frontend.bind("ipc://frontend.ipc");
    backend.bind("ipc://backend.ipc");

    int client_nbr;
    for (client_nbr = 0; client_nbr < 10; client_nbr++)
    {
        pthread_t client;
        pthread_create(&client, NULL, client_thread, NULL);
    }
    int worker_nbr;
    for (worker_nbr = 0; worker_nbr < 3; worker_nbr++)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_thread, NULL);
    }
    //  LRU逻辑
    //  - 一直从backend中获取消息；当有超过一个worker空闲时才从frontend获取消息。
    //  - 当woker回应时，会将该worker标记为已准备好，并转发woker的回应给client
    //  - 如果client发送了请求，就将该请求转发给下一个worker

    //  存放可用worker的队列
    std::queue<std::string> worker_queue;

    while (1)
    {

        //初始化poll集合
        zmq::pollitem_t items[] = {
            //  Always poll for worker activity on backend
            {backend, 0, ZMQ_POLLIN, 0},
            //  Poll front-end only if we have available workers
            {frontend, 0, ZMQ_POLLIN, 0}};

        if (worker_queue.size())
            zmq::poll(&items[0], 2, -1);
        else
            zmq::poll(&items[0], 1, -1);

        //  处理backend中worker的队列
        if (items[0].revents & ZMQ_POLLIN)
        {

            //  将worker的地址入队
            worker_queue.push(s_recv(backend));

            {
                //  跳过空帧
                std::string empty = s_recv(backend);
                assert(empty.size() == 0);
            }

            //  如果是一个应答消息，则转发给client
            std::string client_addr = s_recv(backend);

            //  如果是一个应答消息，则转发给client
            if (client_addr.compare("READY") != 0)
            {

                {
                    std::string empty = s_recv(backend);
                    assert(empty.size() == 0);
                }

                std::string reply = s_recv(backend);
                s_sendmore(frontend, client_addr);
                s_sendmore(frontend, "");
                s_send(frontend, reply);

                if (--client_nbr == 0)
                    break;
            }
        }
        if (items[1].revents & ZMQ_POLLIN)
        {
            //  获取下一个client的请求，交给空闲的worker处理
            //  client请求的消息格式是：[client地址][空帧][请求内容]
            std::string client_addr = s_recv(frontend);

            {
                std::string empty = s_recv(frontend);
                assert(empty.size() == 0);
            }

            std::string request = s_recv(frontend);

            std::string worker_addr = worker_queue.front(); //worker_queue [0];

            //  将该worker的地址出队
            worker_queue.pop();

            s_sendmore(backend, worker_addr);
            s_sendmore(backend, "");
            s_sendmore(backend, client_addr);
            s_sendmore(backend, "");
            s_send(backend, request);
        }
    }
    sleep(1);
    return 0;
}
