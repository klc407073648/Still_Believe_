#ifndef QQ_IPC_H
#define QQ_IPC_H

struct QQ_DATA_INFO {
    int protocal;
    char srcname[20];
    char destname[20];
    char data[100];
};

/*
 * protocal     srcname      destname      data
 * 1            登陆者       NULL
 * 2            发送方       接收方         数据
 * 3            NULL(不在线)
 * 4            退出登陆用户（退出登陆）
 */

#endif
