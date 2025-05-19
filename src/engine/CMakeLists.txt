set(ZFB_ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event.c
)

if (WIN32)
    list(APPEND ZFB_ENGINE_SOURCES
         ${CMAKE_CURRENT_SOURCE_DIR}/key_input/windows.c)
elseif (UNIX AND NOT APPLE)
    list(APPEND ZFB_ENGINE_SOURCES
         ${CMAKE_CURRENT_SOURCE_DIR}/key_input/linux.c)
endif()

add_library(engine OBJECT ${ZFB_ENGINE_SOURCES})
target_include_directories(engine PUBLIC ../headers)
