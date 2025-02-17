cmake_minimum_required(VERSION 3.10)
add_library(renderer STATIC renderer.c)
target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/src/headers)
target_link_libraries(zfb PRIVATE renderer)
