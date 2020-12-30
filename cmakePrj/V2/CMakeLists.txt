CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "g++")  # 设定编译器

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_PROJECT_OPTION STREQUAL "all")
  option(MYPRJ_BUILD_EXAMPLES "Build All examples" ON)
endif()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fpermissive")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -fpermissive")

MESSAGE (STATUS "This is binary dir" ${myprj_BINARY_DIR})
MESSAGE (STATUS "This is source dir" ${myprj_SOURCE_DIR})

# 设定可执行二进制文件和库目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/lib)

#cmake 在find_path和find_library时,会搜索一些默认的路径
#g++在编译过程中，能够搜索到mysql和hiredis的头文件和库文件路径，因为
#这些头文件和库文件都放置/usr/include、/usr/local/include，/usr/local/lib/、/usr/lib64/ 下
#属于g++搜索路径下的内容，即不在需要指定MYSQL_INCLUDE、MYSQL_DIR来添加工程的路径

find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIBRARY NAMES hiredis)

find_path(MYSQL_INCLUDE_DIR mysql/mysql.h)
find_library(MYSQL_LIBRARY NAMES mysqlclient)

find_package(Protobuf)

find_package(ZeroMQ)

if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIBRARY)
  message(STATUS "found hiredis")
endif()

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  message(STATUS "found mysql")
endif()
if(PROTOBUF_FOUND)
  message(STATUS "found protobuf")
endif()

if(ZeroMQ_FOUND)
  message(STATUS "found zeromq")
endif()


#设置是否生成共享库
SET(BUILD_SHARED_LIBS ON)


# 设置各类应用库的头文件
SET(JSONCPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/include)
SET(TINYXML_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/tinyxml/include)
#SET(MYSQL_INCLUDE  /usr/include/mysql)
#SET(HIREDIS_INCLUDE  /usr/local/include/hiredis)
SET(LOG4CPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/include)
SET(COMMON_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/include)
SET(ZEROMQ_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/zeromq/include)

SET(COMMON_SRC_DIR  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/source)

#SET(MINILOG_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/minilog/include)

# 设置各类应用库的lib文件路径
SET(JSONCPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/lib)
SET(TINYXML_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/tinyxml/lib)
#SET(MYSQL_DIR 	/usr/lib64/mysql)
#SET(HIREDIS_DIR /usr/local/lib/)
SET(LOG4CPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/lib)

#SET(MINILOG_DIR ${PROJECT_SOURCE_DIR}/comlib/common/minilog/lib)

# 设置各类应用库的lib链接名称
SET(LIB_JSONCPP jsoncpp)
SET(LIB_TINYXML tinyxml)
SET(LIB_MYSQL mysqlclient)
SET(LIB_HIREDIS hiredis)
SET(LIB_LOG4CPP log4cpp)

#SET(LIB_MINILOG minilog)
SET(LIB_PTHREAD pthread)
SET(LIB_PROTOBUF protobuf)
SET(LIB_PUBSUB pubsub)  
SET(LIB_ZEROMQ zmq)

#LINK_DIRECTORIES(${LOG4CPP_DIR} ${LIB_MINILOG})
LINK_DIRECTORIES(${JSONCPP_DIR} ${TINYXML_DIR}  ${MYSQL_DIR} ${HIREDIS_DIR} ${LOG4CPP_DIR} )

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	SUBDIRS(
				./myprj/login
				./myprj/register
				./myprj/upload
				./myprj/md5
				./myprj/myfiles
				./myprj/dealfile
				./myprj/sharefiles
				./myprj/dealsharefile
	)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	if(MYPRJ_BUILD_EXAMPLES)
		message(STATUS "build all examples")
		SUBDIRS(
					./examples
		)
	else()
		message(STATUS "build only ./examples/ToolClass")
		SUBDIRS(
					./examples/ToolClass/config
					./examples/ToolClass/jsoncpp
					./examples/ToolClass/log4cpp
					./examples/ToolClass/mysql
					./examples/ToolClass/redis
					./examples/ToolClass/tinyxml
		)
	endif()	
endif()

