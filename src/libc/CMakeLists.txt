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
        libc/symtab.hpp
        libc/ast/symtab/symtab.hpp
        libc/ast/symtab/symbols.hpp
        libc/analyzer.hpp
        libc/ast/type_analyzer.hpp
        libc/ast/code_generator.hpp
        libc/code_generator.hpp
    PRIVATE
        libc/dump_tokens.cpp
        libc/parser.cpp
        libc/ast/ast.cpp
        libc/ast/xml_serializer.cpp
        libc/ast/detail/builder.cpp
        libc/ast/detail/builder.hpp
        libc/ast/detail/precedence_builder.cpp
        libc/ast/detail/precedence_builder.hpp
        libc/symtab.cpp
        libc/ast/symtab/symtab.cpp
        libc/ast/symtab/detail/builder.cpp
        libc/ast/symtab/detail/builder.hpp
        libc/analyzer.cpp
        libc/ast/type_analyzer.cpp
        libc/ast/code_generator.cpp
        libc/code_generator.cpp
)

target_link_libraries(
    ${lib_name}
    PRIVATE
        CLexer
        CParser
        pugixml
)