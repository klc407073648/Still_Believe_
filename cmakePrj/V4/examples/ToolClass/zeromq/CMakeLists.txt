#可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TOOLCLASS_EXECUTABLE_OUTPUT_PATH}/zeromq)
set(ZEROMQ_ALL_LIB ${LIB_ZEROMQ} ${LIB_PTHREAD})

include_directories( ${ZEROMQ_INCLUDE}  )

if(ZeroMQ_FOUND)
  add_subdirectory(base_study_demo/hello_world)
  add_subdirectory(base_study_demo/task_vent)
  add_subdirectory(base_study_demo/weather_update)
  add_subdirectory(base_study_demo/multithreaded_relay)
  add_subdirectory(base_study_demo/psenv_pubsub)
  add_subdirectory(base_study_demo/dura_pubsub)
  add_subdirectory(req_rep/mtserver)
  add_subdirectory(req_rep/pthread_use)
  add_subdirectory(req_rep/pthread_use_s)
  add_subdirectory(req_rep/broker)
  add_subdirectory(req_rep/identity)
  add_subdirectory(req_rep/lruqueue)
  add_subdirectory(pub_sub/base_use)
  add_subdirectory(pub_sub/better_use)
  add_subdirectory(pub_sub/sync)
  add_subdirectory(cluster)
  add_subdirectory(Pirate/LazyPirate)
  add_subdirectory(Pirate/SimpleLazyPirate)
  add_subdirectory(Pirate/ParanoidPirate)
else()
  add_subdirectory(base_study_demo/hello_world EXCLUDE_FROM_ALL)
  add_subdirectory(base_study_demo/task_vent EXCLUDE_FROM_ALL)
  add_subdirectory(base_study_demo/weather_update EXCLUDE_FROM_ALL)
  add_subdirectory(base_study_demo/multithreaded_relay EXCLUDE_FROM_ALL)
  add_subdirectory(base_study_demo/psenv_pubsub EXCLUDE_FROM_ALL)
  add_subdirectory(base_study_demo/dura_pubsub EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/mtserver EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/pthread_use EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/pthread_use_s EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/broker EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/identity EXCLUDE_FROM_ALL)
  add_subdirectory(req_rep/lruqueue EXCLUDE_FROM_ALL)
  add_subdirectory(pub_sub/base_use EXCLUDE_FROM_ALL)
  add_subdirectory(pub_sub/better_use EXCLUDE_FROM_ALL)
  add_subdirectory(pub_sub/sync EXCLUDE_FROM_ALL)
  add_subdirectory(cluster EXCLUDE_FROM_ALL)
  add_subdirectory(Pirate/LazyPirate EXCLUDE_FROM_ALL)
  add_subdirectory(Pirate/SimpleLazyPirate EXCLUDE_FROM_ALL)
  add_subdirectory(Pirate/ParanoidPirate EXCLUDE_FROM_ALL)
endif()