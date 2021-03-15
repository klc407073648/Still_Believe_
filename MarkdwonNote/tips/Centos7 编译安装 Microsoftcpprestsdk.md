# Centos7 编译安装 Microsoft/cpprestsdk

## 参考资料

* [Centos7 编译安装 Microsoft/cpprestsdk](https://www.mifaw.com/archives/905)

## 初始化环境

1. 安装编译工具

```
yum group install Development Tools
```
2. 安装依赖库

```
yum install -y zlib-devel libicu-devel openssl-devel
```

3. 安装cmake3

```
cd /home/cmake/
tar -zxf cmake-3.14.5.tar.gz
cd ./cmake-3.14.5/
./bootstrap --prefix=/usr --datadir=share/cmake --docdir=doc/cmake && make
sudo make install
cmake --version
```

4. 安装boost

```
cd /home/boost/
tar -zxf boost_1_73_0.tar.gz
cd boost_1_73_0
#目录可以根据自己的需要配置
./bootstrap.sh --prefix=/usr/local/boost_1_73_0
./b2 install
```

## 编译cpprestsdk

```
cd /home/cpprestsdk/cpprestsdk-master/
git clone https://github.com/Microsoft/cpprestsdk.git
cd cpprestsdk
#这一步很重要
git submodule update --init
mkdir Release/build.release && cd Release/build.release
CC=gcc CXX=g++ cmake .. -DCMAKE_BUILD_TYPE=Release -DWERROR=OFF -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF
make -j 8
sudo make install
```
