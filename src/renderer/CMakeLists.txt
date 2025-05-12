set(ZFB_RENDERER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/renderer.c
)

if(WIN32)
  list(APPEND ZFB_RENDERER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/windows.c
  )
elseif(UNIX AND NOT APPLE)
  list(APPEND ZFB_RENDERER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/linux.c
  )
endif()

add_library(renderer OBJECT ${ZFB_RENDERER_SOURCES})
target_include_directories(renderer PUBLIC ../headers)
