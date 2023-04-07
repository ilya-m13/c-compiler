#pragma once

#include <libc/ast/ast.hpp>

namespace c::ast {

class Visitor {
  public:
    virtual ~Visitor() = default;

    virtual void visit(HeaderFile &node) = 0;
    virtual void visit(FunctionDefinition &node) = 0;

    virtual void visit(Expression &node) = 0;
    virtual void visit(FunctionCall &node) = 0;
    virtual void visit(VariableWriting &node) = 0;
    virtual void visit(DataDeclaration &node) = 0;

    virtual void visit(ReturnStatement &node) = 0;
    virtual void visit(ForStatement &node) = 0;
    virtual void visit(IfStatement &node) = 0;
    virtual void visit(ContinueStatement &node) = 0;
    virtual void visit(BreakStatement &node) = 0;

    virtual void visit(UninitArray &node) = 0;
    virtual void visit(ArrayElementAccess &node) = 0;
    virtual void visit(InitVariable &node) = 0;
    virtual void visit(UninitVariable &node) = 0;
    virtual void visit(Id &node) = 0;

    virtual void visit(Assignment &node) = 0;
    virtual void visit(RvalueOperation &node) = 0;
    virtual void visit(AssignmentOperator &node) = 0;
    virtual void visit(ArithmeticOperator &node) = 0;
    virtual void visit(RelationalOperator &node) = 0;
    virtual void visit(LogicalOperator &node) = 0;
    virtual void visit(PrefixIncrement &node) = 0;
    virtual void visit(PostfixIncrement &node) = 0;
    virtual void visit(PrefixDecrement &node) = 0;
    virtual void visit(PostfixDecrement &node) = 0;

    virtual void visit(PointerType &node) = 0;
    virtual void visit(DataType &node) = 0;
    virtual void visit(BaseType &node) = 0;
    virtual void visit(VoidType &node) = 0;
    virtual void visit(Sign &node) = 0;

    virtual void visit(StringLiteral &node) = 0;
    virtual void visit(IntegerLiteral &node) = 0;
};

} // namespace c::ast