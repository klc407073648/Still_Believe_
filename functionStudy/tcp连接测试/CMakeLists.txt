add_library(pubsub pubsub.cpp codec.cpp TcpClient.cpp)

add_executable(pubClient pub.cpp)
target_link_libraries(pubClient pubsub)

add_executable(subClient sub.cpp)
target_link_libraries(subClient pubsub)

add_executable(HttpServer HttpServer.cpp TcpServer.cpp)

