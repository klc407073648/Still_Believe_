#头文件目录
INCLUDE_DIRECTORIES( ${JSONCPP_INCLUDE} ${TINYXML_INCLUDE} ${MYSQL_INCLUDE} ${HIREDIS_INCLUDE} ${LOG4CPP_INCLUDE}  ./include )

#目录源码路径
SET( LOGIN_SRC_DIR ./source )

#代码列表
FILE (GLOB LOGIN_FILES ${LOGIN_SRC_DIR}/*.cpp )

#依赖的LIB
SET(LOGIN_LIB  ${LIB_JSONCPP} ${LIB_TINYXML}  ${LIB_MYSQL} ${LIB_HIREDIS} ${LIB_LOG4CPP} )

#生成 
ADD_EXECUTABLE( login ${LOGIN_FILES})

#连接生成相关库
TARGET_LINK_LIBRARIES( login ${LOGIN_LIB} -lpthread -lfcgi)