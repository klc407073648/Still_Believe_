#!/bin/bash

# 根据参数设置redis状态
if [[ $1 = "" ]];then
    echo "please input argument:"
    echo "  start   ----->  start redis server"
    echo "  stop    ----->  stop redis server"
    echo "  status  ----->  show the redis server status"
    exit 1
fi


case $1 in
    start)
        # 判断 redis-server 进程是否已经启动...
        ps aux | grep "redis-server" | grep -v grep > /dev/null
        if [ $? -eq 0 ];then
            echo "Redis server is runing ..."
        else
            echo "Redis starting ..."
            redis-server ../conf/redis-6379.conf
            if [ $? -eq 0 ];then
                echo "Redis server start success!!!"
                # 休眠1s, 等待pid文件被创建出来, 再进行后续判断
                sleep 1
            fi
        fi
        ;;
    stop)
        # 判断 redis-server 进程是否已经启动...
        ps aux | grep "redis-server" | grep -v grep > /dev/null
        if [ $? -ne 0 ];then
            echo "Redis server is not runing..."
            exit 1
        fi
        echo "Redis stopping ..."
        # kill redis-server 进程
        PID=$(ps aux | grep "redis-server" | grep -v grep | awk '{print $2}')
        echo Redis server pid = $PID
        kill -9 $PID
        if [ $? -ne 0 ]; then
            echo "Redis server stop fail ..."
        else
            echo "Redis server stop success!!!"
        fi
        ;;
    status)
        ps aux | grep "redis-server" | grep -v grep > /dev/null
        if [ $? -eq 0 ];then
            echo "Redis server is running..."
        else
            echo "Redis server is not running ..."
        fi
        ;;
    *)
        echo "do nothing ..."
        ;;
esac
