
add_executable(syncsub syncsub.cpp )
target_link_libraries( syncsub ${ZEROMQ_ALL_LIB})

add_executable(syncpub syncpub.cpp )
target_link_libraries( syncpub ${ZEROMQ_ALL_LIB})


