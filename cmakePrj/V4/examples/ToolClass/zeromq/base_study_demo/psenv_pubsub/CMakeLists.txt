add_executable(psenvpub psenvpub.cpp )
target_link_libraries( psenvpub ${ZEROMQ_ALL_LIB} )

add_executable(psenvsub psenvsub.cpp)
target_link_libraries( psenvsub ${ZEROMQ_ALL_LIB} )
