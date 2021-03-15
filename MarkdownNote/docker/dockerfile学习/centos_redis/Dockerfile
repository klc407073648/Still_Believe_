FROM centos_base

#install redis
ADD ./package/redis-6.2.0.tar.gz /data/install
ADD ./conf/redis-6379.conf /data
ADD ./install_redis.sh /data/install/install_redis.sh
ADD ./start.sh /data

RUN chmod 755 /data/redis-6379.conf
RUN chmod 755 /data/start.sh
RUN chmod 755 /data/install/install_redis.sh
RUN /data/install/install_redis.sh

ENTRYPOINT /data/start.sh

EXPOSE 6379