docker rm $(docker ps -q -f status=exited)
docker rmi myweb_docker:v1.0

docker build -f ./myweb-dockerfile -t myweb_docker:v1.0 .

chmod 777 ./start.sh
chmod 777 ./conf/myweb-lib.conf
docker run -it myweb_docker:v1.0 bash