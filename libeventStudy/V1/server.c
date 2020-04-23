//server.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <assert.h>
#include <arpa/inet.h>
 
// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg)
{
    char buf[1024] = {0};   
    //从bufferevent的输入缓冲区移除数据
    bufferevent_read(bev, buf, sizeof(buf));
    char* p = "我已经收到了你发送的数据!";
    printf("client say: %s\n", buf);
 
}
 
// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
{
    printf("我是写缓冲区的回调函数...您已发送\n"); 
}
 
// 事件
void event_cb(struct bufferevent *bev, short events, void *arg)
{
    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");  
    }
    else if(events & BEV_EVENT_ERROR)   
    {
        printf("some other error\n");
    }
    
    bufferevent_free(bev);    
    printf("buffevent 资源已经被释放...\n"); 
}
 
 
void send_cb(evutil_socket_t fd, short what, void *arg)
{
    char buf[1024] = {0}; 
    struct bufferevent* bev = (struct bufferevent*)arg;
 //   printf("请输入要发送的数据: \n");
    read(fd, buf, sizeof(buf));
    //向bufferevent的输出缓冲区添加数据
    bufferevent_write(bev, buf, strlen(buf)+1);
}

void cb_listener(
        struct evconnlistener *listener, 
        evutil_socket_t fd, 
        struct sockaddr *addr, 
        int len, void *ptr)
{
  /*   //获取监听的IP和端口信息
    struct sockaddr_in *sock = ( struct sockaddr_in*)&addr;
    
    int port = ntohs(sock->sin_port);
    struct in_addr in  = sock->sin_addr;
	char ip[32];   //INET_ADDRSTRLEN这个宏系统默认定义 16
	//成功的话此时IP地址保存在str字符串中。
	inet_ntop(AF_INET,&in, ip, sizeof(ip));
    //
	printf("connect new client ip:port %s : %d \n",ip,port);
 */
   printf("connect new client \n");
 
   struct event_base* base = (struct event_base*)ptr;
   
   // 通信操作，添加新事件
   struct bufferevent *bev;
   bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
 
   // 给bufferevent缓冲区设置回调
   bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
   bufferevent_enable(bev, EV_READ);
   
   
      // 创建一个事件
      //参数：event_base,监听的对象，需要监听的事件，事件发生后的回调函数，传给回调函数的参数
      //STDIN_FILENO从标准输入，读可持续化EV_READ | EV_PERSIST
    struct event* ev = event_new(base, STDIN_FILENO, 
                                 EV_READ | EV_PERSIST, 
                                 send_cb, bev);
	  event_add(ev, NULL);
   
}


int main(int argc, const char* argv[])
{
 
    // init server 
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9876);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
 
    //创建一个event_base
    struct event_base* base= event_base_new();

    assert(base != NULL);
   
    struct evconnlistener* listener;
    // 创建监听套接字、绑定地址和端口、监听操作(evconnlistener_new)
    listener = evconnlistener_new_bind(base, cb_listener, base, 
                                  LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 
                                  36, (struct sockaddr*)&serv, sizeof(serv));
 
	//event_base_dispatch ()等同于没有设置标志的 event_base_loop()
    //默认情况下,event_base_loop()函数运行 event_base 直到其中没有已经注册的事件为止。
    event_base_dispatch(base);
 
    evconnlistener_free(listener);
    event_base_free(base);
 
    return 0;
}
