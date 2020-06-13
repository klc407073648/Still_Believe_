#include "Fsm.h"
using namespace FsmSpace;
Fsm::Fsm()
{
	eventMap.clear();
	eventRegist(EVENT1, bind(&Fsm::handlerFunc1, this));
	eventRegist(EVENT2, bind(&Fsm::handlerFunc2, this));
	eventRegist(EVENT3, bind(&Fsm::handlerFunc3, this));

	init();
}

Fsm::~Fsm()
{
	
}


void Fsm::eventRegist(int eventID,handlerFunc&& func)
{
	eventMap.insert(make_pair(eventID,std::move(func)));
}
			
void Fsm::init()
{
	running = true ;
	printf("enter Fsm\n ");

	key_t key;/*队列的键值*/
	int ret; /*frok函数返回值*/

	char *msgpath = "/home/believe123/myproject/IPC/V1/";/*消息key产生所用的路径*/
    key = ftok(msgpath,'a');/*产生key*/
	
	if(key != -1)/*产生key成功*/
	{
		printf("成功建立KEY\n");		
	}
	else/*产生key失败*/
	{
		printf("建立KEY失败\n");		
	}

	msg_flags = IPC_CREAT;//|IPC_EXCL;		/*设置创建消息的标志位*/
    msg_qid = msgget(key, msg_flags | 0666); //创建msg_qid的对象

	serviceDeal();
}

void Fsm::serviceDeal()
{
	while(running)
	{
		memset(&msg2, 0, sizeof(msg2));

        if (-1 == msgrcv(msg_qid,(void*)&msg2,sizeof(msg2.mtext),EVENT1,0)){
            printf("receive msg2 failed\n");
            _exit(-1);
        } 
        printf("EVENT1 msg:%s\n",msg2.mtext);
		
		int num = parseEventNumber(msg2.mtext);
		
		if (eventMap.find(num) != eventMap.end())
		{
			(eventMap.find(num)->second)();
		}
		else
		{
			printf("not a register event\n");
		}
	}
}

int Fsm::parseEventNumber(string str)
{
	if (str == "EVENT1")
	{
		return EVENT1;
	}
	else if(str == "EVENT2")
	{
		return EVENT2;
	}
	else if(str == "EVENT3")
	{
		return EVENT3;
	}
	
	return 0;
}

void Fsm::handlerFunc1()
{
	printf("handlerFunc1\n ");
}
void Fsm::handlerFunc2()
{
	printf("handlerFunc2\n ");
}
void Fsm::handlerFunc3()
{
	printf("handlerFunc3\n ");
}