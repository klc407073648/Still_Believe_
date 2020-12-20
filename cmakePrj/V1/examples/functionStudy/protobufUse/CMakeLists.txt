add_custom_command(OUTPUT person.pb.cc person.pb.h
  COMMAND protoc
  ARGS --cpp_out . ${CMAKE_CURRENT_SOURCE_DIR}/person.proto -I${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS person.proto
  VERBATIM )

set_source_files_properties(person.pb.cc PROPERTIES COMPILE_FLAGS "-Wno-conversion -Wno-shadow")
include_directories(${PROJECT_BINARY_DIR}/examples/functionStudy/protobufUse)  

add_library(person_proto person.pb.cc)
target_link_libraries(person_proto ${LIB_PTHREAD} ${LIB_PROTOBUF})

add_executable(functionStudy_test_person test_person.cpp )
set_target_properties(functionStudy_test_person PROPERTIES COMPILE_FLAGS "-Wno-error=shadow")
target_link_libraries(functionStudy_test_person  person_proto)