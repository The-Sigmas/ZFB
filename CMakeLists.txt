cmake_minimum_required(VERSION 3.10)
project(ZFB VERSION 1.0 LANGUAGES C)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(src/headers)
add_subdirectory(src/engine)
add_subdirectory(src/renderer)
add_library(zfb STATIC)
target_sources(zfb PRIVATE src/engine/engine.c src/renderer/renderer.c)
target_include_directories(zfb PUBLIC src/headers)
