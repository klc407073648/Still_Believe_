#include "TcpClient.h"
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <cstring>

using namespace TcpConnect;


void TcpClient::logMsg(const std::string& msg)//后续用va_list
{
  std::cout<<msg<<std::endl;
}

TcpClient::TcpClient(const std::string& ip , int port)
:ip_(ip),port_(port),isRuning_(false)
{
  
   sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

   address_.sin_family = AF_INET;
   address_.sin_addr.s_addr = inet_addr(ip_.c_str());
   address_.sin_port = htons(port_);
   
   logMsg("TcpClient::TcpClient");
}

TcpClient::~TcpClient()
{
   assert(!isRuning_);
   close(sockfd_);
   logMsg("TcpClient::~TcpClient");
}

void TcpClient::connect()
{
	//::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
    ret_ = ::connect(sockfd_, (struct sockaddr *)&address_, sizeof(address_));

    if (ret_ == -1)
    {
      logMsg("oops: client1");
      exit(1);
    }
	
	isRuning_= true;

	connectionCallback_();
    //handleMessage();
}

void TcpClient::disconnect()
{
	close(sockfd_);
}

bool TcpClient::connected()
{
	return isRuning_==true;
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

void TcpClient::send(std::string& msg)
{
	::write(sockfd_, msg.data(), msg.size());
}

void TcpClient::handleMessage()
{
	char buf[1024];
 
    char get[1024] = {0};
	
	int len =0;
	
	while(1)
	{
		fgets(buf, sizeof(buf), stdin);
        int r = Write(sockfd_, buf, strlen(buf));       
        printf("Write r ======== %d\n", r);
        len = Read(sockfd_, buf, sizeof(buf));
        printf("Read len ========= %d\n", len);
        Write(STDOUT_FILENO, buf, len);
	}

	isRuning_=false;
	
	logMsg("TcpClient::handleMessage");
}
