add_executable(req_rep_pthread_use_s pthread_use.cpp )
target_link_libraries( req_rep_pthread_use_s ${LIB_ZEROMQ} ${LIB_PTHREAD})


