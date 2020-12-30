include_directories( ${ZEROMQ_INCLUDE}  )

add_executable(ToolClass_zeromq_pub_sub_better_use_pubServer pubServer.cpp )
target_link_libraries( ToolClass_zeromq_pub_sub_better_use_pubServer ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(ToolClass_zeromq_pub_sub_better_use_subClient subClient.cpp )
target_link_libraries( ToolClass_zeromq_pub_sub_better_use_subClient ${LIB_ZEROMQ} ${LIB_PTHREAD})


