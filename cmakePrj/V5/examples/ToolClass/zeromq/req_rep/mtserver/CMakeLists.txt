add_executable(mtserver mtserver.cpp )
target_link_libraries( mtserver ${ZEROMQ_ALL_LIB})

add_executable(simpleClient simpleClient.cpp )
target_link_libraries( simpleClient ${ZEROMQ_ALL_LIB})


