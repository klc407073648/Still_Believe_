#include "pubsub.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>

using namespace pubsub;

std::vector<string> g_topics;

void subscription(const string& topic, const string& content)
{
  printf("%s: %s\n", topic.c_str(), content.c_str());
}

void connection(PubSubClient* client)
{
  if (client->connected())
  {
    for (std::vector<string>::iterator it = g_topics.begin();
        it != g_topics.end(); ++it)
    {
      client->subscribe(*it, subscription);
    }
  }
  else
  {
    
  }
}

std::vector<std::string> mySplit(const std::string& str, std::string sp_string)  // split(), str 是要分割的string
{ 
    std::vector<std::string> vecString; 
    int sp_stringLen = sp_string.size(); 
    int lastPosition = 0; 
    int index = -1; 
	//如果要分割的串就是自己怎么写？
    while(-1 != (index=str.find(sp_string , lastPosition))) 
    { 
		if(str.substr(lastPosition, index - lastPosition) != std::string("")) //避免第一个字符串就是sp_string
		{
			vecString.push_back(str.substr(lastPosition, index - lastPosition));
		}
        lastPosition = index +sp_stringLen; 
    } 
    std::string lastStr = str.substr(lastPosition); 
    if ( !lastStr.empty() ) 
    { 
        vecString.push_back(lastStr); 
    } 
    return vecString; 
}

int main(int argc, char* argv[])
{
  int opt;
  std::string ip;
  int port;
  std::string topic;  
  const char *str = "i:p:t:";
  while ((opt = getopt(argc, argv, str)) != -1)
  {
    switch (opt)
    {
      case 'i':ip = optarg;break;
      case 'p':port = atoi(optarg);break;
      case 't':topic = optarg;break;
      default:break;
    }
  }
  
  printf("INFO: ip=%s, port=%d, topic=%s",ip.c_str(),port,topic.c_str());

  g_topics=mySplit(topic,"&");

  PubSubClient client(ip, port, "SubClient");
  client.setConnectionCallback(connection);
  client.start();
  while(1);

}
