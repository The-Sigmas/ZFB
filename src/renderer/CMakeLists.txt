set(ZFB_RENDERER_SOURCES
  renderer.c
)
if(WIN32)
  list(APPEND ZFB_RENDERER_SOURCES windows.c)
elseif(UNIX AND NOT APPLE) # See root `CMakeLists.txt` for more information
  list(APPEND ZFB_RENDERER_SOURCES linux.c)
endif()

add_library(renderer STATIC ${ZFB_RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ../headers)
