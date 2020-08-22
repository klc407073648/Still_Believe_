CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "g++")  # 设定编译器

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fpermissive")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -fpermissive")
# 设定可执行二进制文件的目录（最后生成的可执行文件放置的目录）
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/)

# 设置各类应用库的头文件
SET(JSONCPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/include)
SET(TINYXML_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/tinyxml/include)
SET(MYSQL_INCLUDE  /usr/include/mysql)
SET(HIREDIS_INCLUDE  /usr/local/include/hiredis)
SET(LOG4CPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/include)
SET(COMMON_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/include)

SET(COMMON_SRC_DIR  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/source)

#SET(MINILOG_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/minilog/include)

# 设置各类应用库的lib文件路径
SET(JSONCPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/lib)
SET(TINYXML_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/tinyxml/lib)
SET(MYSQL_DIR 	/usr/lib64/mysql)
SET(HIREDIS_DIR /usr/local/lib/)
SET(LOG4CPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/lib)

#SET(MINILOG_DIR ${PROJECT_SOURCE_DIR}/comlib/common/minilog/lib)

# 设置各类应用库的lib链接名称
SET(LIB_JSONCPP jsoncpp)
SET(LIB_TINYXML tinyxml)
SET(LIB_MYSQL mysqlclient)
SET(LIB_HIREDIS hiredis)
SET(LIB_LOG4CPP log4cpp)

#SET(LIB_MINILOG minilog)
#SET(LIB_PTHREAD pthread)

#LINK_DIRECTORIES(${LOG4CPP_DIR} ${LIB_MINILOG})
LINK_DIRECTORIES(${JSONCPP_DIR} ${TINYXML_DIR}  ${MYSQL_DIR} ${HIREDIS_DIR} ${LOG4CPP_DIR} )

SUBDIRS(
			./myprj/login
			./myprj/register
			./myprj/upload
			./myprj/test/jsoncppTset
			./myprj/test/tinyxmlTest
			./myprj/test/redisTest
			./myprj/test/mysqlTest
			./myprj/test/log4cppTest
)

