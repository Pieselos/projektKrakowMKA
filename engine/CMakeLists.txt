add_library(engine SHARED
        src/engine.cpp
)

target_include_directories(engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# potrzebne do __declspec(dllexport)
target_compile_definitions(engine PRIVATE ENGINE_EXPORTS)
