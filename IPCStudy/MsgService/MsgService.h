/**
 * @file msgService.h
 * @brief  message服务
 * @author klc
 * @date 2020-06-14
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_MSGSERVICE_H__
#define __STIBEL_MSGSERVICE_H__

#include <iostream>
#include <string>

#include <iostream>
#include <string>
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <functional>
#include <vector>
#include <map>
#include "ServiceBase.h"

//事件注册: 事件名  事件号
#define EVENT1  1001
#define EVENT2  1002
#define EVENT3  1003

#define MSGSIZE 100
#define MSGTYPE 10001

struct msg_buf
{
    long mtype;
    char mtext[MSGSIZE];
};

using namespace std;

namespace ServiceSpace
{
	class MsgService : public ServiceBase
	{	
		public:
			
			MsgService();
			~MsgService();

			void serviceInit() ;
			
			void listenEventRegist(int eventID,handlerFunc&& func);

			void listenEventRemove(int eventID) ;

			int serviceHandler();
			
			void serviceStop();

	
		private:	
			void init();
    
			void handlerFunc1();
			void handlerFunc2();
			void handlerFunc3();
			
			int parseEventNumber(string str);
			
		private:
			bool  running;
			std::vector<handlerFunc> pHandlerFunc;
			std::map<int , handlerFunc>  eventMap;
			std::map<int , handlerFunc>::iterator eventMapIt;
			int msg_flags, msg_qid;/*创建消息队列函数所用的标志位以及消息队列的id号*/
			key_t key;/*队列的键值*/
			char *msgpath ;/*消息key产生所用的路径*/
			struct msg_buf msg;/*创建消息缓冲区*/
	};
}

#endif //__STIBEL_MSGSERVICE_H__