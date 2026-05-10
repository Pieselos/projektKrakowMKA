
add_executable(app
        main.cpp
        config.h

)

target_link_libraries(app PRIVATE
        engine
        network
        utilities
)
