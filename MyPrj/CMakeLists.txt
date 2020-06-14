CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "g++")  # 设定编译器

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
 
 
# 设定可执行二进制文件的目录（最后生成的可执行文件放置的目录）
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/)

SET(LOG4CPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/include)
SET(JSONCPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/include)
SET(MINILOG_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/minilog/include)

SET(LOG4CPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/lib)
SET(JSONCPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/lib)
SET(MINILOG_DIR ${PROJECT_SOURCE_DIR}/comlib/common/minilog/lib)

SET(LIB_LOG4CPP log4cpp)
SET(LIB_JSONCPP jsoncpp)
SET(LIB_MINILOG minilog)
SET(LIB_PTHREAD pthread)

#LINK_DIRECTORIES(${LOG4CPP_DIR} ${LIB_MINILOG})
LINK_DIRECTORIES(${LOG4CPP_DIR} ${JSONCPP_DIR} ${MINILOG_DIR})

SUBDIRS(
			./myprj/sort
			./myprj/sort1
			./myprj/process
			./myprj/jsontest
			./myprj/msg
)

