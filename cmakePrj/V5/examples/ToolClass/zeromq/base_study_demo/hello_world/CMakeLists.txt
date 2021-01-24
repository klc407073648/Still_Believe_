add_executable(hwclient hwclient.cpp )
target_link_libraries( hwclient ${ZEROMQ_ALL_LIB} )

add_executable(hwserver hwserver.cpp)
target_link_libraries( hwserver ${ZEROMQ_ALL_LIB} )
