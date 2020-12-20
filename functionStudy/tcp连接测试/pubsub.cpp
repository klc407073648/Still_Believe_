#include "codec.h"
#include "pubsub.h"

using namespace pubsub;



PubSubClient::PubSubClient(const string& ip,int port,const string& name)
  : client_(ip, port),name_(name)
{
  client_.setConnectionCallback(
      std::bind(&PubSubClient::onConnection, this));
  client_.setMessageCallback(
      std::bind(&PubSubClient::onMessage, this, _1));
}

void PubSubClient::start()
{
  client_.connect();
}

void PubSubClient::stop()
{
  client_.disconnect();
}

bool PubSubClient::connected() 
{
  return client_.connected();
}

bool PubSubClient::subscribe(const string& topic, const SubscribeCallback& cb)
{
  string message = "sub " + topic + "\r\n";
  subscribeCallback_ = cb;
  return send(message);
}

void PubSubClient::unsubscribe(const string& topic)
{
  string message = "unsub " + topic + "\r\n";
  send(message);
}


bool PubSubClient::publish(const string& topic, const string& content)
{
  string message = "pub " + topic + "\r\n" + content + "\r\n";
  return send(message);
}

void PubSubClient::onConnection()
{
  if (connected()&&connectionCallback_)
  {
    connectionCallback_(this);
  }
}

void PubSubClient::onMessage(string* buf)
{
  ParseResult result = kSuccess;
  while (result == kSuccess)
  {
    string cmd;
    string topic;
    string content;
    result = parseMessage(buf, &cmd, &topic, &content);
    if (result == kSuccess)
    {
      if (cmd == "pub" && subscribeCallback_)
      {
        subscribeCallback_(topic, content);
      }
    }
    else if (result == kError)
    {
      stop();
    }
  }
}

bool PubSubClient::send(string& message)
{
  bool succeed = false;
  if (connected())
  {
    client_.send(message);
    succeed = true;
  }
  return succeed;
}
