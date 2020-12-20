#include "codec.h"

using namespace pubsub;

ParseResult pubsub::parseMessage(std::string* buf,
                         std::string* cmd,
                         std::string* topic,
                         std::string* content)
{
  ParseResult result = kError;
  int pos1stCrlf = buf->find_first_of("\r\n");

  if (pos1stCrlf!=-1)
  {
    int posSpace = buf->find_first_of(" ");
    if (posSpace < pos1stCrlf)
    {
      *cmd=buf->substr(0,posSpace);
      *topic=buf->substr(posSpace+1,pos1stCrlf);
      if (*cmd == "pub")
      {
        int start = pos1stCrlf + 2;
        int pos2stCrlf = buf->find_first_of(start);
        if (pos2stCrlf!=-1)
        {
          *content=buf->substr(start,pos2stCrlf);
           result = kSuccess;
        }
        else
        {
          result = kContinue;
        }
      }
      else
      {
        result = kSuccess;
      }
    }
    else
    {
      result = kError;
    }
  }
  else
  {
    result = kContinue;
  }
  return result;
}

