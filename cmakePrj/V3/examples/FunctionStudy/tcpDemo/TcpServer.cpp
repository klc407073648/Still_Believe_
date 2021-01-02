#include "TcpServer.h"
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include <iostream>
#include <cassert>

using namespace TcpConnect;


void TcpServer::logMsg(const std::string& msg)//后续用va_list
{
  std::cout<<msg<<std::endl;
}

TcpServer::TcpServer(const std::string& ip , int port)
:ip_(ip),port_(port)
{
   sockfd_ = ::socket(AF_INET, SOCK_STREAM, 0);
   assert(sockfd_!=-1);

   serverAddress_.sin_family = AF_INET;
   serverAddress_.sin_addr.s_addr = inet_addr(ip_.c_str());
   serverAddress_.sin_port = htons(port_);
   bzero(&(serverAddress_.sin_zero), 8);

   ret_ = ::bind(sockfd_, (struct sockaddr *)&serverAddress_, sizeof(serverAddress_));
   assert(ret_!=-1);
   
   logMsg("TcpServer::TcpServer");
}

TcpServer::~TcpServer()
{
   
   close(sockfd_);
   logMsg("TcpServer::~TcpServer");
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t n;

again:
	if ( (n = read(fd, ptr, nbytes)) == -1) {
		if (errno == EINTR)
			goto again;
		else
			return -1;
	}

	return n;
}

ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
	ssize_t n;

again:
	if ((n = write(fd, ptr, nbytes)) == -1) {
		if (errno == EINTR)
			goto again;
		else
			return -1;
	}
	return n;
}

void TcpServer::start()
{
	//::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    ret_ = ::listen(sockfd_, 2);

    if (ret_ == -1)
    {
      logMsg("listen fail");
      exit(1);
    
	}
	
	
	struct sockaddr_in clie_addr;
    socklen_t clie_addr_len;
	char clie_IP[1024];
	
	clie_addr_len = sizeof(clie_addr_len);
    clientSockfd_ = ::accept(sockfd_, (struct sockaddr *)&clie_addr, &clie_addr_len);
	
    printf("clientSockfd_ = ----%d\n", clientSockfd_);

    printf("client IP: %s  port:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));

    handleMessage();
}

void TcpServer::handleMessage()
{
	char buf[1024];
	int len,i;
    while (1) {
        len = Read(clientSockfd_, buf, sizeof(buf));
        Write(STDOUT_FILENO, buf, len);

        for (i = 0; i < len; i++)
            buf[i] = toupper(buf[i]);
        Write(clientSockfd_, buf, len); 
    }
	
	logMsg("TcpServer::handleMessage");
}
