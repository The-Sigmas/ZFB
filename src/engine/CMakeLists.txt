set(ZFB_ENGINE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/engine.c
	${CMAKE_CURRENT_SOURCE_DIR}/key_input.c
	${CMAKE_CURRENT_SOURCE_DIR}/event.c
)

add_library(engine OBJECT ${ZFB_ENGINE_SOURCES})
target_include_directories(engine PUBLIC ../headers)
