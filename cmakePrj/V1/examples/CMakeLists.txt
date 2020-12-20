#DataStruct
add_subdirectory(DataStruct/randomArray)
add_subdirectory(DataStruct/skipList)
add_subdirectory(DataStruct/sortAlgorithm)
add_subdirectory(DataStruct/stlUse)

#DesignPattern
add_subdirectory(DesignPattern/AbstractFactory/base_use)
add_subdirectory(DesignPattern/Factory/base_use)
add_subdirectory(DesignPattern/Observer/base_use)
add_subdirectory(DesignPattern/Singleton/base_use)
add_subdirectory(DesignPattern/Singleton/safe_use)
add_subdirectory(DesignPattern/Singleton/share_ptr_use)
add_subdirectory(DesignPattern/Strategy/base_use)
add_subdirectory(DesignPattern/Template/base_use)

#functionStudy
add_subdirectory(functionStudy/functionAndbind)
add_subdirectory(functionStudy/parseCmdLine)
#add_subdirectory(functionStudy/protobufUse)
add_subdirectory(functionStudy/iovecUse)
add_subdirectory(functionStudy/structUse)
add_subdirectory(functionStudy/systemFunUse)
add_subdirectory(functionStudy/tcpDemo)
add_subdirectory(functionStudy/typedefUse)
add_subdirectory(functionStudy/va_listFun)
add_subdirectory(functionStudy/share_ptrUse)
add_subdirectory(functionStudy/scopeFun)

if(PROTOBUF_FOUND)
  add_subdirectory(functionStudy/protobufUse)
else()
  add_subdirectory(functionStudy/protobufUse EXCLUDE_FROM_ALL)
endif()

#ToolClass
add_subdirectory(ToolClass/config)
add_subdirectory(ToolClass/jsoncpp)
add_subdirectory(ToolClass/log4cpp)

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  add_subdirectory(ToolClass/mysql)
else()
  add_subdirectory(ToolClass/mysql EXCLUDE_FROM_ALL)
endif()

if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIBRARY)
  add_subdirectory(ToolClass/redis)
else()
  add_subdirectory(ToolClass/redis EXCLUDE_FROM_ALL)#加上EXCLUDE_FROM_ALL属性就不需要每次编译它
endif()

add_subdirectory(ToolClass/tinyxml)

