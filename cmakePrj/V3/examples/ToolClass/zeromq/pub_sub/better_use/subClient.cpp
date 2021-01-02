#include <zmq.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "zhelpers.hpp"

using namespace std;
std::vector<string> g_topics;

int main(int argc, char *argv[])
{
  if (argc > 2)
  {
    string hostport = argv[1];
    size_t colon = hostport.find(':');
    if (colon != string::npos)
    {
      string hostip = hostport.substr(0, colon);
      uint16_t port = static_cast<uint16_t>(atoi(hostport.c_str() + colon + 1));
      for (int i = 2; i < argc; ++i)
      {
        g_topics.push_back(argv[i]);
      }

      printf("hostport:%s, port:%d\n", hostip.c_str(), port);

      for (int i = 0; i < g_topics.size(); i++)
      {
        printf("%dth topic is %s\n", i + 1, g_topics[i].c_str());
      }

      zmq::context_t context(1);
      zmq::socket_t subscriber(context, ZMQ_SUB);
      //subscriber.connect("tcp://localhost:5556");
      subscriber.connect("tcp://" + hostport);
      for (int i = 0; i < g_topics.size(); i++)
      {
        subscriber.setsockopt(ZMQ_SUBSCRIBE, g_topics[i].c_str(), strlen(g_topics[i].c_str()));
      }

      while (1)
      {
        std::string topic = s_recv(subscriber);
        std::string content = s_recv(subscriber);

        std::cout << "topic:" << topic << " content:" << content << std::endl;
      }
    }
    else
    {
      printf("Usage: %s ip:port topic [content ...]\n", argv[0]);
    }
  }
  else
  {
    printf("Usage: %s ip:port topic [content ...]\n", argv[0]);
  }

  return 0;
}
