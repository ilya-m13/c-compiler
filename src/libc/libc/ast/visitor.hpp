#pragma once

#include <libc/ast/ast.hpp>

namespace c::ast {

class Visitor {
  public:
    virtual ~Visitor() = default;

    virtual void visit(HeaderFile &node) = 0;
    virtual void visit(FunctionDefinition &node) = 0;
    virtual void visit(LocalScope &node) = 0;

    // Expressions

    virtual void visit(Expression &node) = 0;
    virtual void visit(FunctionCall &node) = 0;
    virtual void visit(VariableWriting &node) = 0;
    virtual void visit(DataCreate &node) = 0;

    // Statements

    virtual void visit(ReturnStatement &node) = 0;
    virtual void visit(ForStatement &node) = 0;
    virtual void visit(IfStatement &node) = 0;
    virtual void visit(ContinueStatement &node) = 0;
    virtual void visit(BreakStatement &node) = 0;

    // Struct

    virtual void visit(StructDeclaration &node) = 0;
    virtual void visit(StructDefinition &node) = 0;
    virtual void visit(StructInit &node) = 0;
    virtual void visit(StructUninit &node) = 0;
    virtual void visit(StructElementAccess &node) = 0;
    virtual void visit(StructType &node) = 0;
    virtual void visit(StructElementRefer &node) = 0;

    // Array

    virtual void visit(ArrayUninit &node) = 0;
    virtual void visit(ArrayElementAccess &node) = 0;

    // Variable

    virtual void visit(VariableInit &node) = 0;
    virtual void visit(VariableUninit &node) = 0;
    virtual void visit(VariableAccess &node) = 0;

    // Operations

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

    // Types

    virtual void visit(ArrayType &node) = 0;
    virtual void visit(PointerType &node) = 0;
    virtual void visit(DataType &node) = 0;
    virtual void visit(BaseType &node) = 0;
    virtual void visit(VoidType &node) = 0;

    // Literals

    virtual void visit(StringLiteral &node) = 0;
    virtual void visit(IntegerLiteral &node) = 0;
};

} // namespace c::ast