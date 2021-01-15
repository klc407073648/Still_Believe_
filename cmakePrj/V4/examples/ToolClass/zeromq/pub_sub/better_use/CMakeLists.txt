
add_executable(pubServer pubServer.cpp )
target_link_libraries( pubServer ${ZEROMQ_ALL_LIB})

add_executable(subClient subClient.cpp )
target_link_libraries( subClient ${ZEROMQ_ALL_LIB})


