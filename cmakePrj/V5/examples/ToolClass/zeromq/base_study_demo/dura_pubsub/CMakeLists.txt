add_executable(durapub durapub.cpp )
target_link_libraries( durapub ${ZEROMQ_ALL_LIB} )

add_executable(durasub durasub.cpp)
target_link_libraries( durasub ${ZEROMQ_ALL_LIB} )
