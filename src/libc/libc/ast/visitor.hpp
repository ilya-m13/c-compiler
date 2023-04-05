#pragma once

#include <libc/ast/ast.hpp>

namespace c::ast {

class Visitor {
  public:
    virtual ~Visitor() = default;

    virtual void visit(HeaderFile &node) = 0;
    virtual void visit(FunctionDefinition &node) = 0;

    virtual void visit(FunctionCall &node) = 0;
    virtual void visit(Arg &node) = 0;

    virtual void visit(ReturnStatement &node) = 0;

    virtual void visit(StringLiteral &node) = 0;
    virtual void visit(IntegerLiteral &node) = 0;
};

} // namespace c::ast