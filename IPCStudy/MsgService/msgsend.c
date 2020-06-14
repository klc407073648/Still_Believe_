#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#define MAX_TEXT 512

struct msg_buf
{
    long mtype;
    char mtext[100];
};

int main()
{  
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
    

   
        while(1) 
		{
			
            memset(&msg1, 0 , sizeof(msg1));
          
            scanf("%s",msg1.mtext);
            msg1.mtype = 10001;/*发送消息的类型为MSG_TYPE2，另一个进程收消息的类型应为MSG_TYPE2*/

            //发送消息到消息标识的msg_qid IPC 对象中
            if( -1 == msgsnd(msg_qid,(void *)&msg1,strlen(msg1.mtext),0)) {
                printf("send msg1 failed\n");
                _exit(-1);
            }
         
        }
 
    
    msgctl(msg_qid,IPC_RMID,NULL);

    return 0;
}
