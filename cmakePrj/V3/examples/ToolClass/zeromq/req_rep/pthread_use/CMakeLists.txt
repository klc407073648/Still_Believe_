add_executable(req_rep_pthread_use pthread_use.cpp )
target_link_libraries( req_rep_pthread_use ${LIB_ZEROMQ} ${LIB_PTHREAD})


