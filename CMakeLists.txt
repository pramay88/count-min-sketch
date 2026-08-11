cmake_minimum_required(VERSION 3.20)

project(count_min_sketch
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cms
    src/cms.cpp
)

target_include_directories(cms
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(cms_example
    examples/basic.cpp
)

target_link_libraries(cms_example
    PRIVATE cms
)

enable_testing()
add_subdirectory(test)