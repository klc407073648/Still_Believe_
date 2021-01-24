#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLINE 4096
#define MAXPIPE 16
#define MAXARG 8

struct { 
    char *argv[MAXARG];
    char *in, *out;
} cmd[MAXPIPE+1];

int parse(char *buf, int cmdnum)
{
    int n = 0;
    char *p = buf;
    cmd[cmdnum].in = cmd[cmdnum].out = NULL;

    //ls -l -d -a -F  > out
    while (*p != '\0') {

        if (*p == ' ') {            //将字符串中所有的空格,替换成'\0',方便后续拆分字符串
            *p++ = '\0';
            continue;
        }

        if (*p == '<') {
            *p = '\0';
            while (*(++p) == ' ');    /* cat <     file 处理连续多个空格的情况*/
            cmd[cmdnum].in = p;
            if (*p++ == '\0')
                return -1;
            continue;
        }

        if (*p == '>') {
            *p = '\0';
            while (*(++p) == ' ');
            cmd[cmdnum].out = p;
            if (*p++ == '\0')
                return -1;
            continue;
        }

        if (*p != ' ' && ((p == buf) || *(p-1) == '\0')) {

            if (n < MAXARG - 1) {
                cmd[cmdnum].argv[n++] = p++;   //"ls -l -R > file"
                continue;
            } else {
                return -1;
            }
        }
        p++;
    }

    if (n == 0) {
        return -1;
    }

    cmd[cmdnum].argv[n] = NULL;

    return 0;
}

int main(void)
{
    char buf[MAXLINE];
    pid_t pid;
    int fd, i, j, pfd[MAXPIPE][2], pipe_num, cmd_num;
    char* curcmd, *nextcmd;

    while (1) {
        printf("mysh%% ");
        if (!fgets(buf, MAXLINE, stdin))
            exit(0);
        // "ls -l\n"
        if (buf[strlen(buf)-1]=='\n')
            buf[strlen(buf)-1]='\0';
        cmd_num = 0;
        nextcmd = buf;

        while ((curcmd = strsep(&nextcmd, "|"))) {

            if (parse(curcmd, cmd_num++)<0) {
                cmd_num--;
                break;
            }

            if (cmd_num == MAXPIPE + 1)     
                break;
        }

        if (!cmd_num)
            continue;

        pipe_num = cmd_num - 1;     //根据命令数确定要创建的管道数目

        for (i = 0; i < pipe_num; i++) {    //创建管道
            if (pipe(pfd[i])) {
                perror("pipe");
                exit(1);
            }
        }

        for (i = 0; i < cmd_num; i++) {     //管道数目决定创建子进程个数
            if ((pid = fork()) == 0)
                break;
        }

        if (pid == 0) {
            if (pipe_num) {     //用户输入的命令中含有管道 

                if (i == 0) {                //第一个创建的子进程
                    dup2(pfd[0][1], STDOUT_FILENO);
                    close(pfd[0][0]);

                    for (j = 1; j < pipe_num; j++) { //在该子进程执行期间,关闭该进程使用不到的其他管道的读端和写端
                        close(pfd[j][0]);
                        close(pfd[j][1]);
                    }

                } else if (i==pipe_num) {    //最后一个创建的子进程
                    dup2(pfd[i-1][0], STDIN_FILENO);
                    close(pfd[i-1][1]);

                    for (j = 0; j < pipe_num-1; j++) { //在该子进程执行期间,关闭该进程不使用的其他管道的读/写端
                        close(pfd[j][0]);
                        close(pfd[j][1]);
                    }

                } else {
                    dup2(pfd[i-1][0], STDIN_FILENO);    //重定中间进程的标准输入至管道读端
                    close(pfd[i-1][1]);                 //close管道写端

                    dup2(pfd[i][1], STDOUT_FILENO);     //重定中间进程的标准输出至管道写端
                    close(pfd[i][0]);                   //close管道读端

                    for (j = 0; j < pipe_num; j++)    //关闭不使用的管道读写两端
                        if (j != i || j != i-1) {
                            close(pfd[j][0]);
                            close(pfd[j][1]);
                        }
                }
            }
            if (cmd[i].in) {            /*用户在命令中使用了输入重定向*/
                fd = open(cmd[i].in, O_RDONLY); //打开用户指定的重定向文件,只读即可
                if (fd != -1)
                    dup2(fd, STDIN_FILENO);     //将标准输入重定向给该文件
            }
            if (cmd[i].out) {           /*用户在命令中使用了输出重定向*/
                fd = open(cmd[i].out, O_WRONLY|O_CREAT|O_TRUNC, 0644);  //使用写权限打开用户指定的重定向文件
                if (fd != -1)
                    dup2(fd, STDOUT_FILENO);    //将标准输出重定向给该文件
            }

            execvp(cmd[i].argv[0], cmd[i].argv);    //执行用户输入的命令
            fprintf(stderr, "executing %s error.\n", cmd[i].argv[0]);
            exit(127);
        }

        /*  parent */
        for (i = 0; i < pipe_num; i++) { /*父进程不参与命令执行,关闭其掌握的管道两端*/
            close(pfd[i][0]);
            close(pfd[i][1]);
        }

        for (i = 0; i < cmd_num; i++) { /*循环回首子进程*/
            wait(NULL);
        }
    }
}
