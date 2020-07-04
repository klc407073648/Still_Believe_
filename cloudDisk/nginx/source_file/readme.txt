Nginx的安装：

在正式安装Nginx之前，还需要安装Nginx的三个依赖库：OpenSSL、PCRE和ZLib。其中，OpenSSL用于数据的加密，PCRE用于解析正则表达式，而ZLib用来进行文件的压缩和解压缩。
gcc\g++编译器用来进行编译安装也是必备的。


1、gcc\g++编译器安装

sudo apt-get install gcc
sudo apt-get install g++
sudo apt-get install build-essential
sudo apt-get install libtool

2、PCRE库源码安装

#wget http://downloads.sourceforge.net/project/pcre/pcre/8.39/pcre-8.39.tar.gz
tar -zxvf pcre-8.39.tar.gz
cd pcre-8.39/
./configure
make
sudo make install

3、ZLib库源码安装

tar -zxvf zlib-1.2.8.tar.gz
cd zlib-1.2.8/
./configure
make
sudo make install

4、OpenSSL库源码安装

tar -zxvf openssl-1.0.1t.tar.gz
cd openssl-1.0.1t/
./config
make
sudo make install

5、Nginx源码安装

tar -zxvf nginx-1.10.3.tar.gz
cd nginx-1.10.3/
./configure
make
sudo make install


6.安装好了之后就可以测试一下Nginx是否正确安装了，默认情况下Nginx被安装在目录/usr/local/nginx下。
其中Nginx的配置文件存放在conf/nginx.conf中，bin命令是/sbin目录下的nginx文件。

默认方式启动Nginx
sudo /usr/local/nginx/sbin/nginx
这时，Nginx会自动读取配置文件/usr/loacl/nginx/conf/nginx.conf，并运行；

查看浏览器：
在浏览器搜索栏输入本机的IP，就能看到“Welcome to nginx”，表示Nginx已经安装并运行成功，此时默认配置为一台简单的web服务器。
