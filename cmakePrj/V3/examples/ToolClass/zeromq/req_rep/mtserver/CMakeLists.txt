add_executable(req_rep_mtserver mtserver.cpp )
target_link_libraries( req_rep_mtserver ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(req_rep_simpleClient simpleClient.cpp )
target_link_libraries( req_rep_simpleClient ${LIB_ZEROMQ} ${LIB_PTHREAD})


