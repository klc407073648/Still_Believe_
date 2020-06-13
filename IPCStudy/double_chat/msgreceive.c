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
    key_t key = 12345; //个人使用的时候可以直接指定key
    
	
	//创建msg_qid的对象
    int msg_qid = msgget(key, IPC_CREAT | 0666);
    struct msg_buf msg1,msg2;

    int ret;

    ret = fork();
    if ( -1 == ret ) {
        printf("fork failed! \n");
        _exit(-1);
    }
    else if (ret == 0) {
        while(1) {
            memset(&msg1, 0 , sizeof(msg1));
            
            //初始化消息类型以及消息内容
            scanf("%s",msg1.mtext);
            msg1.mtype = MSG_TYPE2;

            //发送消息到消息标识的msg_qid IPC 对象中
            /*if( -1 == msgsnd(msg_qid,(void *)&msg1,strlen(msg1.mtext),0)) {
                printf("send msg1 failed\n");
                _exit(-1);
            }*/
            msgsnd(msg_qid,&msg1,100,0);
        }
    }
    else {
        while(1) { 
            memset(&msg2, 0, sizeof(msg2));

            if (-1 == msgrcv(msg_qid,(void*)&msg2,sizeof(msg2.mtext),MSG_TYPE1,0)){
                printf("receive msg2 failed\n");
                _exit(-1);
            } 
            printf("client2:%s\n",msg2.mtext);
        }
    }
    msgctl(msg_qid,IPC_RMID,NULL);

    return 0;
}
