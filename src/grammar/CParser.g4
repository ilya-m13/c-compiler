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
    | struct_declaration
    | struct_definition
    ;

header_file
    : INCLUDE HEADER_FILE
    ;

function_definition
    : return_type ID OPENPAR CLOSEPAR LBRACE action* RBRACE
    | return_type ID OPENPAR arg_declaration (COMMA arg_declaration)* CLOSEPAR
        LBRACE action* RBRACE
    ;

return_type
    : data_type
    | pointer_type
    | struct_type
    | void_type
    ;

action
    : expression
    | statement
    | local_scope
    ;

local_scope
    : LBRACE action* RBRACE
    ;

// expressions

expression
    : function_call SEMICOLON
    | variable_writing SEMICOLON
    | data_create SEMICOLON
    ;

function_call
    : ID OPENPAR CLOSEPAR
    | ID OPENPAR arg (COMMA arg)* CLOSEPAR
    ;

variable_writing
    : assignment
    | increment
    | decrement
    ;

data_create
    : variable_create
    | array_create
    | struct_create
    ;

// statements

statement
    : return_statement
    | for_statement
    | if_statement
    | continue_statement
    | break_statement
    ;

return_statement
    : RETURN value SEMICOLON
    ;

for_statement
    : FOR OPENPAR for_data_using? SEMICOLON truth_value? SEMICOLON value? 
        CLOSEPAR LBRACE action* RBRACE
    ;

for_data_using
    : data_create
    | variable_writing
    ;

if_statement
    : IF OPENPAR truth_value CLOSEPAR LBRACE action* RBRACE
    ;

continue_statement
    : CONTINUE SEMICOLON
    ;

break_statement
    : BREAK SEMICOLON
    ;

// value

truth_value
    : value
    ;

value
    : lvalue
    | rvalue_expression
    | variable_writing
    ;

rvalue_expression
    : rvalue
    | rvalue_operation
    ;

lvalue
    : variable_access
    | array_element_access
    | struct_element_access
    ;

rvalue
    : literal
    | function_call
    | increment
    | decrement
    ;

// object

data_uninit
    : variable_uninit
    | array_uninit
    | struct_uninit
    ;

// struct

struct_declaration
    : STRUCT ID SEMICOLON
    ;

struct_definition
    : STRUCT ID LBRACE (data_uninit SEMICOLON)+ RBRACE ID? SEMICOLON
    ;

struct_create
    : struct_uninit
    | struct_init
    ;

struct_init
    : struct_type ID ASSIGN LBRACE value (COMMA value)* RBRACE
    ;

struct_uninit
    : struct_type ID
    ;

struct_element_access
    : struct_lvalue (struct_element_refer struct_lvalue)+
    ;

struct_type
    : CONST? STRUCT ID
    ;

struct_lvalue
    : variable_access
    | array_element_access
    ;

struct_element_refer
    : PERIOD
    ;

// array

array_create
    : array_uninit
    ;

// TODO: array_init (size is rvalue without lvalue)

array_uninit
    : sign? base_type ID LBRACKET value RBRACKET
    ;

array_element_access
    : ID LBRACKET value RBRACKET
    ;

// variable 

variable_create
    : variable_init
    | variable_uninit
    ;

variable_init
    : variable_type ID ASSIGN value
    ;

variable_uninit
    : variable_type ID
    ;

variable_type
    : data_type
    | pointer_type
    ;

variable_access
    : ID
    ;

// operations

assignment
    : (lvalue assignment_operator)+ (rvalue_expression | lvalue)
    ;

rvalue_operation
    : ((lvalue | rvalue) operator)+ (lvalue | rvalue)
    ;

// TODO: misc
operator
    : arithmetic_operator
    | relational_operator
    | logical_operator
    ;

assignment_operator
    : ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | MULTIP_ASSIGN
    | DIV_ASSIGN
    | MODULO_ASSIGN
    ;

arithmetic_operator
    : ADD
    | SUB
    | MULTIP
    | DIV
    | MODULO
    ;

relational_operator
    : EQUAL
    | NOT_EQUAL
    | GREATER_THAN
    | LESS_THAN
    | GREATER_EQUAL
    | LESS_EQUAL
    ;

logical_operator
    : NOT
    | AND
    | OR
    ;

increment
    : prefix_increment
    | postfix_increment
    ;

decrement
    : prefix_decrement
    | postfix_decrement
    ;

prefix_increment
    : INCREMENT lvalue
    ;

postfix_increment
    : lvalue INCREMENT
    ;

prefix_decrement
    : DECREMENT lvalue
    ;

postfix_decrement
    : lvalue DECREMENT
    ;

// types

pointer_type
    : CONST? sign? any_type MULTIP MULTIP*
    ;

any_type
    : base_type
    | void_type
    | struct_type
    ;

data_type
    : CONST? sign? base_type
    ;

base_type
    : INT
    | CHAR
    | FLOAT
    | DOUBLE
    | SHORT
    | LONG
    ;

void_type
    : VOID
    ;

sign
    : UNSIGNED
    | SIGNED
    ;

// literals

literal
    : string_literal
    | integer_literal
    ;

string_literal
    : STRING
    ;

integer_literal
    : INTEGER
    ;

// args

arg_declaration
    : data_uninit
    ;

arg
    : value
    ;