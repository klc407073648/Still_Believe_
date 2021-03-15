#!/bin/bash

cd /data/install/redis-6.2.0
make
make PREFIX=/usr/local/redis install 

cp /data/redis-6379.conf /usr/local/redis/

##不适用默认配置文件
#cp redis.conf /usr/local/redis/
#sed -i "s/daemonize on/daemonize yes/g" /usr/local/redis/redis.conf
#sed -i "s/protected mode yes/protected mode no/g" /usr/local/redis/redis.conf
#sed -i "s/# requirepass foobared/requirepass 123456/g" /usr/local/redis/redis.conf

# requirepass foobared
#/usr/local/redis/bin/redis-server /usr/local/redis/redis.conf