set(ZFB_RENDERER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer.c)

if(WIN32)
  list(APPEND ZFB_RENDERER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/windows.c)
elseif(UNIX AND NOT APPLE) # See root `CMakeLists.txt` for more information
  list(APPEND ZFB_RENDERER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/linux.c)
endif()

#list(APPEND ZFB_RENDERER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/renderer.c)

add_library(renderer STATIC ${ZFB_RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ../headers)
