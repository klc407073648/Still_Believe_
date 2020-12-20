#ifndef __STIBEL_PUBSUB_H__
#define __STIBEL_PUBSUB_H__

#include "TcpClient.h"
#include <functional>
#include "noncopyable.h"

using namespace std;
using namespace TcpConnect;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


namespace pubsub
{

class PubSubClient : noncopyable
{
 public:
  typedef std::function<void (PubSubClient*)> ConnectionCallback;
  typedef std::function<void (const string& topic,
                              const string& content)> SubscribeCallback;

  PubSubClient(const string& ip,int port,const string& name);

  void start();
  void stop();
  bool connected();

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  bool subscribe(const string& topic, const SubscribeCallback& cb);
  void unsubscribe(const string& topic);
  bool publish(const string& topic, const string& content);

 private:
  void onConnection();
  void onMessage(string* buf);
  bool send(string& message);

  TcpClient client_;
  std::string name_;
  ConnectionCallback connectionCallback_;
  SubscribeCallback subscribeCallback_;
};
}  // namespace pubsub

#endif  // __STIBEL_PUBSUB_H__
