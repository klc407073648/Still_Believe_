add_executable(req_rep_hwclient hwclient.cpp )
target_link_libraries( req_rep_hwclient ${LIB_ZEROMQ} )

add_executable(req_rep_hwserver hwserver.cpp)
target_link_libraries( req_rep_hwserver ${LIB_ZEROMQ} )
