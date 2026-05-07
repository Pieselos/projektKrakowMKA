find_package(CURL REQUIRED)

add_library(network STATIC
        fetchTransitData.cpp
)

target_include_directories(network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(network PUBLIC CURL::libcurl)