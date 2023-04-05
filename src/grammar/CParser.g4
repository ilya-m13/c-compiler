parser grammar CParser;

options {
	tokenVocab = CLexer;
}

// $antlr-format false

program
    : element* EOF
    ;

element
    : header_file
    | function_definition
    ;

header_file
    : INCLUDE HEADER_FILE
    ;

function_definition
    : return_type ID OPENPAR CLOSEPAR LBRACE action* RBRACE
    ;

return_type
    : INT
    ;

action
    : expression
    | statement
    ;

expression
    : function_call SEMICOLON
    ;

function_call
    : ID OPENPAR CLOSEPAR
    | ID OPENPAR arg (COMMA arg)* CLOSEPAR
    ;

arg
    : string_literal
    ;

statement
    : return_statement
    ;

return_statement
    : RETURN integer_literal SEMICOLON
    ;

string_literal
    : STRING
    ;

integer_literal
    : INTEGER
    ;