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

    std::any
    visitFunction_call(CParser::Function_callContext *context) override;
    std::any visitArg(CParser::ArgContext *context) override;

    std::any
    visitReturn_statement(CParser::Return_statementContext *context) override;

    std::any
    visitString_literal(CParser::String_literalContext *context) override;
    std::any
    visitInteger_literal(CParser::Integer_literalContext *context) override;

  private:
    Program &program_;
};

} // namespace c::ast::detail