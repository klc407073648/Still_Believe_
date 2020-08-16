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

# **************************** 杀死正在运行的CGI进程 **************************** 
if [ "$STOP" -eq 1 ];then
    # 登录
    kill -9 $(ps aux | grep "./deploy/login" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 注册
    kill -9 $(ps aux | grep "./deploy/register" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 上传文件
    kill -9 $(ps aux | grep "./deploy/upload" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # MD5 秒传
    kill -9 $(ps aux | grep "./deploy/md5" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 我的文件
    kill -9 $(ps aux | grep "./deploy/myfiles" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 分享删除文件
    kill -9 $(ps aux | grep "./deploy/dealfile" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 共享文件列表
    kill -9 $(ps aux | grep "./deploy/sharefiles" | grep -v grep | awk '{print $2}') > /dev/null 2>&1
    # 共享文件pv字段处理、取消分享、转存文件
    kill -9 $(ps aux | grep "./deploy/dealsharefile" | grep -v grep | awk '{print $2}') > /dev/null 2>&1

    echo "CGI 程序已经成功关闭, bye-bye ..."

fi

cd ..
# ******************************* 重新启动CGI进程 ******************************* 
if [ "$START" -eq 1 ];then
    # 登录
    echo -n "登录："
    spawn-fcgi -a 127.0.0.1 -p 10000 -f ./deploy/login
    # 注册
    echo -n "注册："
    spawn-fcgi -a 127.0.0.1 -p 10001 -f ./deploy/register
    # 上传文件
    echo -n "上传："
    spawn-fcgi -a 127.0.0.1 -p 10002 -f ./deploy/upload
    # MD5秒传
    echo -n "MD5："
    spawn-fcgi -a 127.0.0.1 -p 10003 -f ./deploy/md5
    # 我的文件
    echo -n "MyFile："
    spawn-fcgi -a 127.0.0.1 -p 10004 -f ./deploy/myfiles
    # 分享删除文件
    echo -n "DealFile："
    spawn-fcgi -a 127.0.0.1 -p 10005 -f ./deploy/dealfile
    # 共享文件列表
    echo -n "ShareList："
    spawn-fcgi -a 127.0.0.1 -p 10006 -f ./deploy/sharefiles
    # 共享文件pv字段处理、取消分享、转存文件
    echo -n "DealShare："
    spawn-fcgi -a 127.0.0.1 -p 10007 -f ./deploy/dealsharefile

    echo "CGI 程序已经成功启动 ^_^..."
fi

cd -
