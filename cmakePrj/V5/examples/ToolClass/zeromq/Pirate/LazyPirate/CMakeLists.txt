
add_executable(lpclient lpclient.cpp )
target_link_libraries( lpclient ${ZEROMQ_ALL_LIB})

add_executable(lpserver lpserver.cpp )
target_link_libraries( lpserver ${ZEROMQ_ALL_LIB})


