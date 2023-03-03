set(lib_name c)

add_library(
    ${lib_name}
    STATIC
        libc/dump_tokens.cpp
)

include(CompileOptions)
set_compile_options(${lib_name})

target_include_directories(${lib_name} PUBLIC .)

target_link_libraries(
    ${lib_name}
    PRIVATE
    CLexer   
)