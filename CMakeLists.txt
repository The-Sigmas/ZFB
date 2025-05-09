cmake_minimum_required(VERSION 3.10)
project(ZFB VERSION 1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src/headers)

if(WIN32) # For now expects it to be installed via mingw
  include_directories("C:/msys64/mingw64/include/libpng16")
  link_directories("C:/msys64/mingw64/lib")
endif()

add_subdirectory(src/engine)
add_subdirectory(src/renderer)

set(ZFB_SOURCES
  src/zfb.c
)

add_library(ZFB STATIC ${ZFB_SOURCES})

if(WIN32)
  target_link_libraries(ZFB PRIVATE engine renderer png16)
else()
  target_link_libraries(ZFB PRIVATE engine renderer)
endif()
target_include_directories(ZFB PUBLIC src/headers)
