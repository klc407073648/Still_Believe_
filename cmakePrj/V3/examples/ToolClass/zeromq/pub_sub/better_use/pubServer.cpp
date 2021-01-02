
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "zhelpers.hpp"

using namespace std;
std::vector<string> g_topics;
std::vector<string> g_contents;

int main(int argc, char *argv[])
{

  if (argc >= 4 && argc % 2 == 0)
  {
    string hostport = argv[1];
    size_t colon = hostport.find(':');
    if (colon != string::npos)
    {
      string hostip = hostport.substr(0, colon);
      uint16_t port = static_cast<uint16_t>(atoi(hostport.c_str() + colon + 1));

      for (int i = 2; i < argc; i++)
      {
        if (i % 2 == 0)
        {
          g_topics.push_back(argv[i]);
        }
        else
        {
          g_contents.push_back(argv[i]);
        }
      }

      printf("hostport:%s, port:%d\n", hostip.c_str(), port);

      for (int i = 0; i < g_topics.size(); i++)
      {
        printf("%dth g_topic is %s, g_content is %s\n", i + 1, g_topics[i].c_str(), g_contents[i].c_str());
      }

      zmq::context_t context(1);
      zmq::socket_t publisher(context, ZMQ_PUB);
      //publisher.bind("tcp://*:5556");
      publisher.bind("tcp://" + hostport);
      publisher.bind("ipc://weather.ipc");

      while (1)
      {
        for (int i = 0; i < g_topics.size(); i++)
        {
          s_sendmore(publisher, g_topics[i]);
          s_send(publisher, g_contents[i]);
        }
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
