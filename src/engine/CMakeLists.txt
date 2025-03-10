add_library(engine STATIC engine.c key_input.c)
target_include_directories(engine PUBLIC ../headers)
