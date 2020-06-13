#ifndef _FSM_H_
#define _FSM_H_

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

#define EVENT1  1001
#define EVENT2  1002
#define EVENT3  1003

struct msg_buf
{
    long mtype;
    char mtext[100];
};

using namespace std;

namespace FsmSpace
{
	class Fsm
	{	
		public:
			
			Fsm();
			~Fsm();

			typedef std::function<void()> handlerFunc;

			void eventRegist(int eventID,handlerFunc&& func);
			
			void init();
            void serviceDeal();

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
			struct msg_buf msg1,msg2;/*创建消息缓冲区*/
	};
}

#endif //_FSM_H_