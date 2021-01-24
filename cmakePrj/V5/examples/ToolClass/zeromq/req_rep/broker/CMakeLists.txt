add_executable(rrbroker rrbroker.cpp )
target_link_libraries( rrbroker ${ZEROMQ_ALL_LIB})

add_executable(rrserver rrserver.cpp )
target_link_libraries( rrserver ${ZEROMQ_ALL_LIB})

add_executable(rrclient rrclient.cpp )
target_link_libraries( rrclient ${ZEROMQ_ALL_LIB})

add_executable(msgqueue msgqueue.cpp )
target_link_libraries( msgqueue ${ZEROMQ_ALL_LIB})


