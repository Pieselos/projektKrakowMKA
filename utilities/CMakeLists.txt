
add_library(utilities STATIC
        utilities.cpp
        ../third_party/miniz/miniz.c
)

target_include_directories(utilities PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../third_party/miniz
        ../engine/include/engine
)
