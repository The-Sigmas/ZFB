cmake_minimum_required(VERSION 3.10)
add_library(engine STATIC engine.c)
target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/src/headers)
target_link_libraries(zfb PRIVATE engine)

