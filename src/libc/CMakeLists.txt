set(lib_name c)

add_library(
    ${lib_name}
    STATIC
)

include(CompileOptions)
set_compile_options(${lib_name})

target_include_directories(${lib_name} PUBLIC .)

target_sources(
    ${lib_name}
    PUBLIC
        libc/dump_tokens.hpp
        libc/parser.hpp
        libc/ast/ast.hpp
        libc/ast/visitor.hpp
        libc/ast/xml_serializer.hpp
    PRIVATE
        libc/dump_tokens.cpp
        libc/parser.cpp
        libc/ast/ast.cpp
        libc/ast/xml_serializer.cpp
        libc/ast/detail/builder.cpp
        libc/ast/detail/precedence_builder.cpp
        libc/ast/detail/builder.hpp
        libc/ast/detail/precedence_builder.hpp
)

target_link_libraries(
    ${lib_name}
    PRIVATE
        CLexer
        CParser
        pugixml
)