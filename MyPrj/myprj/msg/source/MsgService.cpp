#include "MsgService.h"
#include "MsgServiceUtil.h"
using namespace ServiceSpace;

MsgService::MsgService()
{

	listenEventRegist(EVENT1, bind(&MsgService::handlerFunc1, this));
	listenEventRegist(EVENT2, bind(&MsgService::handlerFunc2, this));
	listenEventRegist(EVENT3, bind(&MsgService::handlerFunc3, this));


	listenEventRemove(EVENT2);

	serviceInit();
}

MsgService::~MsgService()
{
	serviceStop();
}


void MsgService::listenEventRegist(int eventID,handlerFunc&& func)
{
	eventMap.insert(make_pair(eventID,std::move(func)));
}

void MsgService::listenEventRemove(int eventID)
{
	eventMapIt=eventMap.find(eventID);
	if(eventMapIt!=eventMap.end())
	{
		eventMap.erase(eventID);
	}
	
}

void MsgService::serviceInit()
{
	printf("MsgService::serviceInit()\n ");
	init();
	serviceHandler();
}

void MsgService::init()
{
	printf("MsgService::init()\n ");
	running = true ;

	memset(&msg, 0, sizeof(msg));

	msgpath = "/home/believe123/myproject/IPC/V1/";/*消息key产生所用的路径*/
    key = ftok(msgpath,'a');/*产生key*/
	
	if(key != -1)
		printf("成功建立KEY\n");		
	else
		printf("建立KEY失败\n");		

	msg_flags = IPC_CREAT;//|IPC_EXCL;		/*设置创建消息的标志位*/
    msg_qid = msgget(key, msg_flags | 0666); //创建msg_qid的对象
}

int MsgService::serviceHandler()
{
	printf("MsgService::serviceHandler()\n ");
	while(running)
	{
		memset(&msg, 0, sizeof(msg));

        if (-1 == msgrcv(msg_qid,(void*)&msg,sizeof(msg.mtext),MSGTYPE,0)){
            printf("receive MSGTYPE failed\n");
            _exit(-1);
        } 
        printf("msg content:%s\n",msg.mtext);
		
		//暂时还未实现json消息的发送
		/* string str = "{\"EVENT\" : 1001, \"MSG\" : \"hello world\"}";
		TypeUtil::jsonStrParse(str); */
		int num = parseEventNumber(msg.mtext);
		
		if (eventMap.find(num) != eventMap.end())
		{
			(eventMap.find(num)->second)();
		}
		else
		{
			printf("not a register event\n");
		}
	}
	
	return 0;
}

int MsgService::parseEventNumber(string str)
{
	printf("MsgService::parseEventNumber()\n ");
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

void MsgService::handlerFunc1()
{
	printf("handlerFunc1\n ");
}
void MsgService::handlerFunc2()
{
	printf("handlerFunc2\n ");
}
void MsgService::handlerFunc3()
{
	printf("handlerFunc3\n ");
}

void MsgService::serviceStop()
{
	printf("MsgService::serviceStop()\n ");
}