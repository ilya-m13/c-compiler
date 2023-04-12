find_package(ANTLR REQUIRED)

set(lexer_lib_name CLexer)
add_library(${lexer_lib_name} STATIC)
set(antlr_lexer_target_name Antlr${lexer_lib_name})
antlr_target(
    ${antlr_lexer_target_name}
    ${lexer_lib_name}.g4
    LEXER
)
set_target_properties(
    ${lexer_lib_name}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
target_sources(
    ${lexer_lib_name}
    PRIVATE
        ${ANTLR_${antlr_lexer_target_name}_CXX_OUTPUTS}
)
target_include_directories(
    ${lexer_lib_name}
    PUBLIC
        ${ANTLR_${antlr_lexer_target_name}_OUTPUT_DIR}
)
target_link_libraries(
    ${lexer_lib_name}
    PUBLIC
        antlr4
)

set(parser_lib_name CParser)
add_library(${parser_lib_name} STATIC)
set(antlr_parser_target_name Antlr${parser_lib_name})
antlr_target(
    ${antlr_parser_target_name}
    ${parser_lib_name}.g4
    PARSER
    VISITOR
    DEPENDS_ANTLR ${antlr_lexer_target_name}
    COMPILE_FLAGS -lib ${ANTLR_${antlr_lexer_target_name}_OUTPUT_DIR}
)
set_target_properties(
    ${parser_lib_name}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)
target_sources(
    ${parser_lib_name}
    PRIVATE
        ${ANTLR_${antlr_parser_target_name}_CXX_OUTPUTS}
)
target_include_directories(
    ${parser_lib_name}
    PUBLIC
        ${ANTLR_${antlr_parser_target_name}_OUTPUT_DIR}
)
target_link_libraries(
    ${parser_lib_name}
    PUBLIC
        antlr4
)