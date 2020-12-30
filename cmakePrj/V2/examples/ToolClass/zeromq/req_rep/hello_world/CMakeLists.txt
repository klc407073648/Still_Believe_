add_executable(ToolClass_zeromq_req_rep_hwclient hwclient.cpp )
target_link_libraries( ToolClass_zeromq_req_rep_hwclient ${LIB_ZEROMQ} )

add_executable(ToolClass_zeromq_req_rep_hwserver hwserver.cpp)
target_link_libraries( ToolClass_zeromq_req_rep_hwserver ${LIB_ZEROMQ} )
