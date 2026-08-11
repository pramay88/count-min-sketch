add_executable(cms_test
    test.cpp
)

target_link_libraries(cms_test
    PRIVATE cms
)

add_test(
    NAME cms_random_stream
    COMMAND cms_test
)