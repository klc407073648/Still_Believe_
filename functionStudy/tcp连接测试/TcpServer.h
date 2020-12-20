#ifndef __STIBEL_TCPSERVER_H__
#define __STIBEL_TCPSERVER_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#define SERVER_PORT 10000
#define BUFFER_SIZE 1024

namespace TcpConnect
{

class TcpServer
{
  public:
      TcpServer(const std::string& ip="127.0.0.1" , int port=SERVER_PORT);
      ~TcpServer();

      void start();
      
  private:
      void handleMessage();
	  void logMsg(const std::string& msg);
  
  private:
      std::string ip_;
      int port_;
	  
      int sockfd_;
	  int clientSockfd_;
	  struct sockaddr_in serverAddress_;
	  
      int ret_;
};


} // namespace pubsub

#endif  // __STIBEL_TCPSERVER_H__
