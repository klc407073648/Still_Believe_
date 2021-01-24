add_executable(base_pubServer pubServer.cpp )
target_link_libraries( base_pubServer ${ZEROMQ_ALL_LIB})

add_executable(base_subClient subClient.cpp )
target_link_libraries( base_subClient ${ZEROMQ_ALL_LIB})


