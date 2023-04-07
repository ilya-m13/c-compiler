#include <libc/ast/ast.hpp>
#include <libc/ast/visitor.hpp>

namespace c::ast {

void HeaderFile::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionDefinition::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Expression::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void VariableWriting::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void DataDeclaration::accept(Visitor &visitor) {
    visitor.visit(*this);
}

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

void UninitArray::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ArrayElementAccess::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void InitVariable::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void UninitVariable::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Id::accept(Visitor &visitor) {
    visitor.visit(*this);
}

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

void StringLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void IntegerLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

} // namespace c::ast