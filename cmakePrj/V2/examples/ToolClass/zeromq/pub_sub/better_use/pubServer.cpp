
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "zhelpers.hpp"

using namespace std;
string g_topic;
string g_content;

int main (int argc, char* argv[]) {

  if (argc == 4)
  {
    string hostport = argv[1];
    size_t colon = hostport.find(':');
    if (colon != string::npos)
    {
      string hostip = hostport.substr(0, colon);
      uint16_t port = static_cast<uint16_t>(atoi(hostport.c_str()+colon+1));
      g_topic = argv[2];
      g_content = argv[3];

      printf("hostport:%s, port:%d\n",hostip.c_str(),port);

      printf("g_topic is %s, g_content is %s\n",g_topic.c_str(),g_content.c_str());


      zmq::context_t context (1);
      zmq::socket_t publisher (context, ZMQ_PUB);
      //publisher.bind("tcp://*:5556");
      publisher.bind("tcp://"+hostport);
      publisher.bind("ipc://weather.ipc");
      
      while (1) {

        s_sendmore (publisher, g_topic);
        s_send (publisher, g_content);

        sleep(1);
       }
    }
    else
    {
      printf("Usage: %s ip:port topic content\n", argv[0]);
    }
  }
  else
  {
    printf("Usage: %s ip:port topic content\n", argv[0]);
  }
    
  return 0;
}
