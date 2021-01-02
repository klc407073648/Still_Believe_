
add_executable(pub_sub_syncsub syncsub.cpp )
target_link_libraries( pub_sub_syncsub ${LIB_ZEROMQ} ${LIB_PTHREAD})

add_executable(pub_sub_syncpub syncpub.cpp )
target_link_libraries( pub_sub_syncpub ${LIB_ZEROMQ} ${LIB_PTHREAD})


