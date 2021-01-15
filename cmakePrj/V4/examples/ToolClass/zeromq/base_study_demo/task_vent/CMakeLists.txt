add_executable(tasksink tasksink.cpp )
target_link_libraries( tasksink ${ZEROMQ_ALL_LIB} )

add_executable(taskvent taskvent.cpp)
target_link_libraries( taskvent ${ZEROMQ_ALL_LIB} )

add_executable(taskwork taskwork.cpp)
target_link_libraries( taskwork ${ZEROMQ_ALL_LIB} )
