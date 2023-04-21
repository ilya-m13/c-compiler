#include <libc/ast/ast.hpp>
#include <libc/ast/visitor.hpp>

namespace c::ast {

void HeaderFile::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionDefinition::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void LocalScope::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Expressions

void Expression::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void VariableWriting::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void DataCreate::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Statements

void ReturnStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ForStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void IfStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ContinueStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void BreakStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Struct

void StructDeclaration::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructDefinition::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructInit::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructUninit::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructElementAccess::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StructElementRefer::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Array

void ArrayUninit::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ArrayElementAccess::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Variable

void VariableInit::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void VariableUninit::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void VariableAccess::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Operations

void Assignment::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void RvalueOperation::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void AssignmentOperator::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ArithmeticOperator::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void RelationalOperator::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void LogicalOperator::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void PrefixIncrement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void PostfixIncrement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void PrefixDecrement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void PostfixDecrement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Types

void ArrayType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void PointerType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void DataType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void BaseType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void VoidType::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Sign::accept(Visitor &visitor) {
    visitor.visit(*this);
}

// Literals

void StringLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void IntegerLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

} // namespace c::ast