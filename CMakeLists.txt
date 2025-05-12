cmake_minimum_required(VERSION 3.10)
project(ZFB VERSION 1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src/headers)

add_subdirectory(extern/zlib)

set(PNG_STATIC TRUE)
set(SKIP_INSTALL_ALL TRUE)
set(ZLIB_LIBRARY zlib)
set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/zlib")
add_subdirectory(extern/libpng)

add_subdirectory(src/engine)
add_subdirectory(src/renderer)

set(ZFB_SOURCES
  src/zfb.c
)

add_library(ZFB STATIC
  ${ZFB_SOURCES}
  $<TARGET_OBJECTS:engine>
  $<TARGET_OBJECTS:renderer>
)

target_link_libraries(ZFB PRIVATE png zlib)
target_include_directories(ZFB PUBLIC src/headers)
