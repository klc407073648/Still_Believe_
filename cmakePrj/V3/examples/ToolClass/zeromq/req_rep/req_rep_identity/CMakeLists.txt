add_executable(identity identity.cpp )
target_link_libraries( identity ${LIB_ZEROMQ} ${LIB_PTHREAD})


