#ifndef __STIBEL_TCPCONNECT_H__
#define __STIBEL_TCPCONNECT_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <functional>

namespace TcpConnect
{

class TcpClient
{
  public:
	  typedef std::function<void ()> ConnectionCallback;
	  typedef std::function<void (std::string*)> MessageCallback;
      TcpClient(const std::string& ip="127.0.0.1" , int port=10000);
      ~TcpClient();

      void connect();
      void disconnect();
      bool connected();
	  void send(std::string& msg);
	  void setConnectionCallback(ConnectionCallback cb)
		{ connectionCallback_ = std::move(cb); }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(MessageCallback cb)
  { messageCallback_ = std::move(cb); }
      
  private:
      void handleMessage();
	  void logMsg(const std::string& msg);
  
  private:
      std::string ip_;
      int port_;
	  
      int sockfd_;
	  struct sockaddr_in address_;
	  
      int ret_;
	  bool isRuning_;
	  
	  ConnectionCallback connectionCallback_;
	  MessageCallback messageCallback_;
};


} // namespace pubsub

#endif  // __STIBEL_TCPCONNECT_H__
