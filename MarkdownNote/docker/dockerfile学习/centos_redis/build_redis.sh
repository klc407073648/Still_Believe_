#/bin/bash
docker rm $(docker ps -q -f status=exited)
docker rmi centos_redis

dos2unix *.sh
chmod 777 *.sh

docker build -t centos_redis .

#docker run -itd -p 8379:6379 --name redis centos_redis bash
