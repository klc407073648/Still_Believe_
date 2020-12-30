add_library(pubsub SHARED pubsub.cpp codec.cpp TcpClient.cpp)

add_executable(functionStudy_pubClient pub.cpp)
target_link_libraries(functionStudy_pubClient ${LIB_PUBSUB})

add_executable(functionStudy_subClient sub.cpp)
target_link_libraries(functionStudy_subClient ${LIB_PUBSUB})

add_executable(functionStudy_HttpServer HttpServer.cpp TcpServer.cpp)

