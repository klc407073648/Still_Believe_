CMAKE_MINIMUM_REQUIRED(VERSION 2.8)  # 设定版本
PROJECT(myprj) # 设定工程名
SET(CMAKE_CXX_COMPILER "gcc")  # 设定编译器

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fpermissive")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -g -fpermissive")
# 设定可执行二进制文件的目录（最后生成的可执行文件放置的目录）
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/deploy/)

# 设置各类应用库的头文件
SET(FASTDFS_INCLUDE  /usr/include/fastdfs)
SET(FASTCOMMON_INCLUDE  /usr/include/fastcommon)
SET(HIREDIS_INCLUDE  /usr/local/include/hiredis)
SET(MYSQL_INCLUDE  /usr/include/mysql)
SET(COMMON_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/include)
SET(LOG4CPP_INCLUDE  ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/include)

SET(COMMON_SRC_DIR  ${PROJECT_SOURCE_DIR}/comlib/common/fastcgi/source)

# 设置各类应用库的lib文件路径
#SET(JSONCPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/jsoncpp/lib)
#SET(TINYXML_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/tinyxml/lib)
SET(MYSQL_DIR 	/usr/lib64/mysql)
SET(HIREDIS_DIR /usr/local/lib/)
#SET(LOG4CPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/lib)
SET(LOG4CPP_DIR ${PROJECT_SOURCE_DIR}/comlib/3part/log4cpp/lib)


# 设置各类应用库的lib链接名称
SET(LIB_FASTDFS fdfsclient)
SET(LIB_FASTCOMMON fastcommon)
SET(LIB_HIREDIS hiredis)
SET(LIB_FCGI fcgi)
SET(LIB_M m)
SET(LIB_MYSQL mysqlclient)
SET(LIB_LOG4CPP log4cpp)

LINK_DIRECTORIES(${MYSQL_DIR} ${HIREDIS_DIR} ${LOG4CPP_DIR} )

SUBDIRS(
			./myprj/login
			./myprj/register
			./myprj/md5
			./myprj/upload
			./myprj/myfiles
			./myprj/dealfile
			./myprj/dealsharefile
			./myprj/sharefiles
)

