add_executable(wuclient wuclient.cpp )
target_link_libraries( wuclient ${ZEROMQ_ALL_LIB} )

add_executable(wuserver wuserver.cpp)
target_link_libraries( wuserver ${ZEROMQ_ALL_LIB} )
