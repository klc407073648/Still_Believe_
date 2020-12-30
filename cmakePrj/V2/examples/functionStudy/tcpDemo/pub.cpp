#include "pubsub.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>


using namespace pubsub;

string g_topic;
string g_content;

void connection(PubSubClient* client)
{
  if (client->connected())
  {
    client->publish(g_topic, g_content);
    client->stop();
  }
  else
  {

  }
}

int main(int argc, char* argv[])
{
  int opt;
  std::string ip;
  int port;
  std::string topic;  
  std::string content;  
  
  const char *str = "i:p:t:c:";
  while ((opt = getopt(argc, argv, str)) != -1)
  {
    switch (opt)
    {
      case 'i':ip = optarg;break;
      case 'p': port = atoi(optarg);break;
      case 't':topic = optarg;break;
      case 'c':content = optarg;break;
      default:break;
    }
  }
  
  printf("INFO: ip=%s, port=%d, topic=%s, content=%s",ip.c_str(),port,topic.c_str(),content.c_str());
 
  g_topic=topic;
  g_content=content;

  PubSubClient client(ip, port, "PubClient");
  client.setConnectionCallback(connection);
  client.start();

  while(1);
}
