#ifndef __STIBEL_CODEC_H__
#define __STIBEL_CODEC_H__

#include <string>

namespace pubsub
{

enum ParseResult
{
  kError,
  kSuccess,
  kContinue,
};

ParseResult parseMessage(std::string* buf,
                         std::string* cmd,
                         std::string* topic,
                         std::string* content);
}  // namespace pubsub

#endif  // __STIBEL_CODEC_H__

