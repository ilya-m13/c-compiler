set(app_name c-compiler)

add_executable(${app_name})

include(CompileOptions)
set_compile_options(${app_name})

target_sources(
    ${app_name}
    PRIVATE
        ${app_name}/main.cpp
)

target_link_libraries(
    ${app_name}
    PRIVATE
        cxxopts
        c
)