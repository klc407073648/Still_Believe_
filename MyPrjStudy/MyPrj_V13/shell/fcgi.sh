#!/bin/bash

START=1
STOP=1

case $1 in
    start)
        START=1
        STOP=0
        ;;
    stop)
        START=0
        STOP=1
        ;;
    "")
        STOP=1
        START=1
        ;;
    *)
        STOP=0
        START=0
        ;;
esac


cd ..
# **************************** 杀死正在运行的CGI进程 **************************** 
if [ "$STOP" -eq 1 ];then
    # kill fcgi服务 
    kill -9 $(ps aux | grep "./deploy/fastcgiTEST" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
fi


# ******************************* 重新启动CGI进程 ******************************* 
if [ "$START" -eq 1 ];then
    # start fcgi服务
    echo -n "facgi start："
    spawn-fcgi  -a 127.0.0.1 -p 8050 -f ./deploy/fastcgiTEST
    echo "CGI 程序已经成功启动 ^_^..."
fi

cd -
