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

    std::any visitExpression(CParser::ExpressionContext *context) override;
    std::any
    visitFunction_call(CParser::Function_callContext *context) override;
    std::any
    visitVariable_writing(CParser::Variable_writingContext *context) override;
    std::any
    visitData_declaration(CParser::Data_declarationContext *context) override;

    std::any
    visitReturn_statement(CParser::Return_statementContext *context) override;
    std::any
    visitFor_statement(CParser::For_statementContext *context) override;
    std::any visitIf_statement(CParser::If_statementContext *context) override;
    std::any visitContinue_statement(
        CParser::Continue_statementContext *context) override;
    std::any
    visitBreak_statement(CParser::Break_statementContext *context) override;

    std::any visitUninit_array(CParser::Uninit_arrayContext *context) override;
    std::any visitArray_element_access(
        CParser::Array_element_accessContext *context) override;
    std::any
    visitInit_variable(CParser::Init_variableContext *context) override;
    std::any
    visitUninit_variable(CParser::Uninit_variableContext *context) override;
    std::any visitId(CParser::IdContext *context) override;

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

    std::any visitPointer_type(CParser::Pointer_typeContext *context) override;
    std::any visitData_type(CParser::Data_typeContext *context) override;
    std::any visitBase_type(CParser::Base_typeContext *context) override;
    std::any visitVoid_type(CParser::Void_typeContext *context) override;
    std::any visitSign(CParser::SignContext *context) override;

    std::any
    visitString_literal(CParser::String_literalContext *context) override;
    std::any
    visitInteger_literal(CParser::Integer_literalContext *context) override;

  private:
    Program &program_;
};

} // namespace c::ast::detail