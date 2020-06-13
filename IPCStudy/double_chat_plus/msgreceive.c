#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_TYPE1 1
#define MSG_TYPE2 2 

struct msg_buf
{
    long mtype;
    char mtext[100];
};

int main()
{
	//当多用户的时候通过指定文件以及设置id来获取唯一的key标识
    //key_t key = ftok(".",100);
    //key_t key = 12345; //个人使用的时候可以直接指定key
    
	int msg_flags, msg_qid;/*创建消息队列函数所用的标志位以及消息队列的id号*/
	key_t key;/*队列的键值*/
	struct msg_buf msg1,msg2;/*创建消息缓冲区*/
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
    

    ret = fork();
    if ( -1 == ret ) {
        printf("fork failed! \n");
        _exit(-1);
    }
    else if (ret == 0) /*子进程负责发送MSG_TYPE2类型的消息*/
	{
        while(1) 
		{
			
            memset(&msg1, 0 , sizeof(msg1));
          
            scanf("%s",msg1.mtext);
            msg1.mtype = MSG_TYPE2;/*发送消息的类型为MSG_TYPE2，另一个进程收消息的类型应为MSG_TYPE2*/

            //发送消息到消息标识的msg_qid IPC 对象中
            if( -1 == msgsnd(msg_qid,(void *)&msg1,strlen(msg1.mtext),0)) {
                printf("send msg1 failed\n");
                _exit(-1);
            }
         
        }
    }
    else {
        while(1) /*主进程负责发送MSG_TYPE1类型的消息*/
		{ 
            memset(&msg2, 0, sizeof(msg2));

            if (-1 == msgrcv(msg_qid,(void*)&msg2,sizeof(msg2.mtext),MSG_TYPE1,0)){
                printf("receive msg2 failed\n");
                _exit(-1);
            } 
            printf("MSG_TYPE1 msg:%s\n",msg2.mtext);
        }
    }
    msgctl(msg_qid,IPC_RMID,NULL);

    return 0;
}
