/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file Event.h
/// @brief http请求路由
///
/// @version 1.0
/// @author liukang
/// @date 2017.12.15
//////////////////////////////////////////////
#pragma once

#include "Request.h"
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <pthread.h>
#include <string>
#include"LoggerManager.h"
#include "MySQL.h"

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
    static MySQL::ptr getMySQLInstance() {return  m_MySQL;}

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
    static MySQL::ptr m_MySQL;
    EventMap m_EventMap;
    int m_iThreadNum;
};
