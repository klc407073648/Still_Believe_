add_library(pubsub SHARED pubsub.cpp codec.cpp TcpClient.cpp)

add_executable(pubClient pub.cpp)
target_link_libraries(pubClient ${LIB_PUBSUB})

add_executable(subClient sub.cpp)
target_link_libraries(subClient ${LIB_PUBSUB})

add_executable(HttpServer HttpServer.cpp TcpServer.cpp)

