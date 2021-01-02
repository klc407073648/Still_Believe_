add_executable(rrbroker rrbroker.cpp )
target_link_libraries( rrbroker ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(rrserver rrserver.cpp )
target_link_libraries( rrserver ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(rrclient rrclient.cpp )
target_link_libraries( rrclient ${LIB_ZEROMQ} ${LIB_PTHREAD})


