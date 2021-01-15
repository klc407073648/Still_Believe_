
add_executable(pp_lpclient lpclient.cpp )
target_link_libraries( pp_lpclient ${ZEROMQ_ALL_LIB})

add_executable(ppqueue ppqueue.cpp )
target_link_libraries( ppqueue ${ZEROMQ_ALL_LIB})

add_executable(ppworker ppworker.cpp )
target_link_libraries( ppworker ${ZEROMQ_ALL_LIB})


