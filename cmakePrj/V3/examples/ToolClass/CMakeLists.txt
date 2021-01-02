#ToolClass
add_subdirectory(config)
add_subdirectory(jsoncpp)
add_subdirectory(log4cpp)

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  add_subdirectory(mysql)
else()
  add_subdirectory(mysql EXCLUDE_FROM_ALL)
endif()

if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIBRARY)
  add_subdirectory(redis)
else()
  add_subdirectory(redis EXCLUDE_FROM_ALL)#加上EXCLUDE_FROM_ALL属性就不需要每次编译它
endif()

add_subdirectory(tinyxml)

if(ZeroMQ_FOUND)
  add_subdirectory(zeromq)
else()
  add_subdirectory(zeromq EXCLUDE_FROM_ALL)

endif()

