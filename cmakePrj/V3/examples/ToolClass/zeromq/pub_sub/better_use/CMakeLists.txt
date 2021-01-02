
add_executable(pub_sub_better_use_pubServer pubServer.cpp )
target_link_libraries( pub_sub_better_use_pubServer ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(pub_sub_better_use_subClient subClient.cpp )
target_link_libraries( pub_sub_better_use_subClient ${LIB_ZEROMQ} ${LIB_PTHREAD})


