#!/bin/bash
MYSQL_DB="myredis"
MYSQL_PWD="klczxas789"
FLAG="-N -r"
SQLSHELL="events_to_redis.sql"
IP="127.0.0.1"
PORT="6379"
mysql -uroot -p${MYSQL_PWD} -D${MYSQL_DB} ${FLAG} <  ${SQLSHELL} | redis-cli -h ${IP} -p ${PORT} --pipe 


