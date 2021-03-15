# 制作redis的docker镜像

## 参考资料

* [Docker创建redis容器，且可以通过宿主机访问](https://www.jianshu.com/p/a75b4b6b8460)
* [docker服务，构建centos基础镜像和开发环境镜像Dockerfile脚本 ](https://github.com/cmxiaocai/)


## 制作redis的docker镜像的文件介绍

```
yum install tree

[root@master01 centos_redis]#  tree
.
|-- build_redis.sh          #使用docker build -t centos_redis . 制作以Dockerfile文件为基础的镜像
|-- conf
|   |-- redis-6379.conf     # 使用的redis配置文件
|   `-- redis.conf		    # 原始配置文件
|-- Dockerfile
|-- install_redis.sh	    # redis的安装脚本
|-- package
|   `-- redis-6.2.0.tar.gz  # redis的安装包
`-- start.sh				# docker run 时候的默认启动脚本，ENTRYPOINT /data/start.sh


整个构建过程，直接执行./build_redis.sh 即可
```

## 实践

不作端口映射，直接访问小网的端口 172.18.0.5:6379
```
[root@master01 centos_redis]# docker run -itd centos_redis bash
74fd1d1b98e5ae68a359b6b84d99e856c045eec8e73412180910a1f86ae0196a

[root@master01 centos_redis]# docker ps |grep centos_redis
74fd1d1b98e5        centos_redis                                                  18 seconds ago      Up 17 seconds       6379/tcp            compassionate_mahavi

宿主机操作：
[root@master01 bin]# ./redis-cli -h 172.18.0.5 -p 6379
172.18.0.5:6379> auth 123456
OK
172.18.0.5:6379> keys *
(empty array)
172.18.0.5:6379> set name kkk
OK
172.18.0.5:6379> get num
"123"
172.18.0.5:6379> keys *
1) "num"
2) "name"

容器内操作：
[root@master01 centos_redis]# docker exec -it 74fd1d1b98e5 bash
[root@74fd1d1b98e5 /]# ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 02:51 ?        00:00:00 /bin/sh -c /data/start.sh bash
root         6     1  0 02:51 ?        00:00:00 /usr/local/redis/bin/redis-server
root        11     0  0 02:51 ?        00:00:00 bash
root        26    11  0 02:52 ?        00:00:00 ps -ef
[root@74fd1d1b98e5 /]# cd /usr/local/redis/bin/
[root@74fd1d1b98e5 bin]# ./redis-cli
127.0.0.1:6379> auth 123456
OK
127.0.0.1:6379> keys *
1) "name"
127.0.0.1:6379> set num 123
OK
127.0.0.1:6379> get name
"kkk"

```

将宿主机的8379端口映射到容器里的6379端口
```
[root@master01 centos_redis]# docker run -itd -p 8379:6379 centos_redis bash
096df055afc64566f7cd023b4db6b76d81abe141a2706908878bc0be581cb87c

[root@master01 centos_redis]# docker ps |grep centos
096df055afc6        centos_redis                                                 "/bin/sh -c /data/..."   7 seconds ago       Up 7 seconds        0.0.0.0:8379->6379/tcp   xenodochial_newton
74fd1d1b98e5        centos_redis                                                 "/bin/sh -c /data/..."   19 minutes ago      Up 19 minutes       6379/tcp                 compassionate_mahavira

宿主机操作：
[root@master01 centos_redis]# cd /usr/local/redis/bin/
[root@master01 bin]# ./redis-cli  -h 127.0.0.1 -p 8379
127.0.0.1:8379> keys *
(error) NOAUTH Authentication required.
127.0.0.1:8379> auth 123456
OK
127.0.0.1:8379> keys *
(empty array)
127.0.0.1:8379> set name json
OK
127.0.0.1:8379> get num
"44444"
127.0.0.1:8379>


容器内操作：
[root@master01 ~]# docker exec -it 096df055afc6 bash
[root@096df055afc6 /]# cd /usr/local/redis/bin/
[root@096df055afc6 bin]# ./redis-cli
127.0.0.1:6379> auth 123456
OK
127.0.0.1:6379> keys *
1) "name"
127.0.0.1:6379> set num 44444
OK
127.0.0.1:6379>

```

## 问题记录

```
问题1：
redis Error: Connection reset by peer:

解答1：
# bind 127.0.0.1
需要注释掉。因为这个是设置redis服务器只在本地监听，从而会拒绝来自外网的监听，同时：

protected-mode no 
去掉注释，设置为 no,表明不已保护模式运行。

requirepass YOURPASSWORD 
设置远程访问密码


docker save -o centos_base.tar centos_base:latest

docker load -i centos_base.tar
```

