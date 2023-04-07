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
    | return_type ID OPENPAR arg_declaration (COMMA arg_declaration)* CLOSEPAR
        LBRACE action* RBRACE
    ;

action
    : expression
    | statement
    ;

// expressions

expression
    : function_call SEMICOLON
    | variable_writing SEMICOLON
    | data_declaration SEMICOLON
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

data_declaration
    : variable_declaration
    | array_declaration
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
    : FOR OPENPAR data_declaration? SEMICOLON truth_value? SEMICOLON value? 
        CLOSEPAR LBRACE action* RBRACE
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
    : id
    | array_element_access
    ;

rvalue
    : literal
    | function_call
    | increment
    | decrement
    ;

// array

// TODO: init_array (size is rvalue without lvalue)
array_declaration
    : uninit_array
    ;

uninit_array
    : sign? base_type ID LBRACKET value RBRACKET
    ;

array_element_access
    : ID LBRACKET value RBRACKET
    ;

// variable 

variable_declaration
    : init_variable
    | uninit_variable
    ;

init_variable
    : (data_type | pointer_type) ID ASSIGN value
    ;

uninit_variable
    : (data_type | pointer_type) ID
    ;

id
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

return_type
    : data_type
    | pointer_type
    ;

pointer_type
    : CONST? sign? any_type MULTIP MULTIP*
    ;

data_type
    : CONST? sign? base_type
    ;

any_type
    : base_type
    | void_type
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
    : uninit_variable
    ;

arg
    : value
    ;