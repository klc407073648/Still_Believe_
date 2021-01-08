
add_executable(lpclient lpclient.cpp )
target_link_libraries( lpclient ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(lpserver lpserver.cpp )
target_link_libraries( lpserver ${LIB_ZEROMQ} ${LIB_PTHREAD})


