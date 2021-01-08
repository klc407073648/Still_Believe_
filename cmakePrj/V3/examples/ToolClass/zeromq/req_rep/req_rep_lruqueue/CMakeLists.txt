add_executable(lruqueue lruqueue.cpp )
target_link_libraries( lruqueue ${LIB_ZEROMQ} ${LIB_PTHREAD})


