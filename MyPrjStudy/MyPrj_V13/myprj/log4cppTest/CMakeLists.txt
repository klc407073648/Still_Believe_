#头文件目录
INCLUDE_DIRECTORIES( ${LOG4CPP_INCLUDE}  ./include )

#目录源码路径
SET( LOG4CPPTEST_SRC_DIR ./source )

#代码列表
FILE (GLOB LOG4CPPTEST_FILES ${LOG4CPPTEST_SRC_DIR}/*.cpp)

#依赖的LIB
SET(LOG4CPPTEST_LIB  ${LIB_LOG4CPP} )

#生成 
ADD_EXECUTABLE( log4cppTEST ${LOG4CPPTEST_FILES})

#连接生成相关库
TARGET_LINK_LIBRARIES( log4cppTEST ${LOG4CPPTEST_LIB} -lpthread)