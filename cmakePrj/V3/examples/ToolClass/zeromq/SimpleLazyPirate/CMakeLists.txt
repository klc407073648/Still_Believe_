
add_executable(spqueue spqueue.cpp )
target_link_libraries( spqueue ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(spworker spworker.cpp )
target_link_libraries( spworker ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(sp_client simpleClient.cpp )
target_link_libraries( sp_client ${LIB_ZEROMQ} ${LIB_PTHREAD})


