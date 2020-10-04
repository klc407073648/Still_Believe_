/**
 * @file Event.h
 * @brief 事件类封装，通过addEvent进行web请求的监听
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_EVENT_H__
#define __STIBEL_EVENT_H__


#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <pthread.h>
#include <string>
#include "Request.h"
#include"LoggerManager.h"
#include "TEncode.h"

using namespace EncodeFun;
using namespace minilog;

#define eventBind(funcName, appName) std::bind(&funcName, &appName, std::placeholders::_1)

typedef std::function<Response(Request)> handlerFunc;
typedef std::map<string , handlerFunc> EventMap;

class Event;

struct threadStruct{
    Event* pthis;
    int threadID;
};

class  Event
{
public:
    Event(int threadNum = 1);
    ~Event();

    void addEvent(string requestEvent, handlerFunc  func);
	char* memstr(char* full_data, int full_data_len, char* substr);
    void exec();

private:
    DECLARE_STATIC_LOGGERMANAGER();
    //处理消息的线程函数
    static inline void *pthreadTask(void *p)
    {
        threadStruct* data = (threadStruct*)p;
        (static_cast<Event*>(data->pthis))->processMessage(data->threadID);
        return NULL;
    }
    void processMessage(int threadID); 

private:
    
    EventMap m_EventMap;
    int m_iThreadNum;
};

#endif //__STIBEL_EVENT_H__
