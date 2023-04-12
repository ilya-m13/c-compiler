#pragma once

#include <libc/ast/ast.hpp>

#include <CParserBaseVisitor.h>

#include <any>

namespace c::ast::detail {

class Builder final : public CParserBaseVisitor {
  public:
    explicit Builder(ast::Program &program) : program_(program) {}

    std::any visitProgram(CParser::ProgramContext *context) override;

    std::any visitHeader_file(CParser::Header_fileContext *context) override;
    std::any visitFunction_definition(
        CParser::Function_definitionContext *context) override;

    // Expressions

    std::any visitExpression(CParser::ExpressionContext *context) override;
    std::any
    visitFunction_call(CParser::Function_callContext *context) override;
    std::any
    visitVariable_writing(CParser::Variable_writingContext *context) override;
    std::any visitData_create(CParser::Data_createContext *context) override;

    // Statements

    std::any
    visitReturn_statement(CParser::Return_statementContext *context) override;
    std::any
    visitFor_statement(CParser::For_statementContext *context) override;
    std::any visitIf_statement(CParser::If_statementContext *context) override;
    std::any visitContinue_statement(
        CParser::Continue_statementContext *context) override;
    std::any
    visitBreak_statement(CParser::Break_statementContext *context) override;

    // Struct

    std::any visitStruct_declaration(
        CParser::Struct_declarationContext *context) override;
    std::any
    visitStruct_definition(CParser::Struct_definitionContext *context) override;
    std::any visitStruct_init(CParser::Struct_initContext *context) override;
    std::any
    visitStruct_uninit(CParser::Struct_uninitContext *context) override;
    std::any visitStruct_element_access(
        CParser::Struct_element_accessContext *context) override;
    std::any visitStruct_type(CParser::Struct_typeContext *context) override;
    std::any visitStruct_element_refer(
        CParser::Struct_element_referContext *context) override;

    // Array

    std::any visitArray_uninit(CParser::Array_uninitContext *context) override;
    std::any visitArray_element_access(
        CParser::Array_element_accessContext *context) override;

    // Variable

    std::any
    visitVariable_init(CParser::Variable_initContext *context) override;
    std::any
    visitVariable_uninit(CParser::Variable_uninitContext *context) override;
    std::any
    visitVariable_access(CParser::Variable_accessContext *context) override;

    // Operations

    std::any visitAssignment(CParser::AssignmentContext *context) override;
    std::any
    visitRvalue_operation(CParser::Rvalue_operationContext *context) override;
    std::any visitAssignment_operator(
        CParser::Assignment_operatorContext *context) override;
    std::any visitArithmetic_operator(
        CParser::Arithmetic_operatorContext *context) override;
    std::any visitRelational_operator(
        CParser::Relational_operatorContext *context) override;
    std::any
    visitLogical_operator(CParser::Logical_operatorContext *context) override;
    std::any
    visitPrefix_increment(CParser::Prefix_incrementContext *context) override;
    std::any
    visitPostfix_increment(CParser::Postfix_incrementContext *context) override;
    std::any
    visitPrefix_decrement(CParser::Prefix_decrementContext *context) override;
    std::any
    visitPostfix_decrement(CParser::Postfix_decrementContext *context) override;

    // Types

    std::any visitPointer_type(CParser::Pointer_typeContext *context) override;
    std::any visitData_type(CParser::Data_typeContext *context) override;
    std::any visitBase_type(CParser::Base_typeContext *context) override;
    std::any visitVoid_type(CParser::Void_typeContext *context) override;
    std::any visitSign(CParser::SignContext *context) override;

    // Literals

    std::any
    visitString_literal(CParser::String_literalContext *context) override;
    std::any
    visitInteger_literal(CParser::Integer_literalContext *context) override;

  private:
    Program &program_;
};

} // namespace c::ast::detail