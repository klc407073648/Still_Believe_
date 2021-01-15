
add_executable(spqueue spqueue.cpp )
target_link_libraries( spqueue ${ZEROMQ_ALL_LIB})

add_executable(spworker spworker.cpp )
target_link_libraries( spworker ${ZEROMQ_ALL_LIB})

add_executable(sp_client simpleClient.cpp )
target_link_libraries( sp_client ${ZEROMQ_ALL_LIB})


