CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "g++")  # 设定编译器

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -fpermissive")
 
# 设定可执行二进制文件的目录（最后生成的可执行文件放置的目录）
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy)

SET(JSONCPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/include)
SET(JSONCPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/lib)
SET(LIB_JSONCPP jsoncpp)
 
# 设定链接目录
LINK_DIRECTORIES(${JSONCPP_DIR} )
 
SUBDIRS(
            ${PROJECT_SOURCE_DIR}/myprj
		)



