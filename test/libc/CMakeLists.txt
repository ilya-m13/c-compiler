enable_testing()

include(CompileOptions)
include(GoogleTest)

set(test_name tests)
add_executable(${test_name})
set_compile_options(${test_name})
target_include_directories(
    ${test_name}
    PRIVATE
        .
        ${CMAKE_BINARY_DIR}/test/libc
)
target_sources(
    ${test_name}
    PRIVATE
        libc/dump_tokens.test.cpp
        libc/parser.test.cpp
)
target_link_libraries(
    ${test_name}
    PRIVATE
        c
        GTest::gtest_main
)
gtest_discover_tests(${test_name})