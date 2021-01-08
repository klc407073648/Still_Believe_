
add_executable(cluster_peer1 peering1.cpp )
target_link_libraries( cluster_peer1 ${LIB_ZEROMQ} ${LIB_PTHREAD})



