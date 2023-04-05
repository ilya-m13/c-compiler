#include <libc/ast/ast.hpp>
#include <libc/ast/visitor.hpp>

namespace c::ast {

void HeaderFile::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionDefinition::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Arg::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void ReturnStatement::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void StringLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void IntegerLiteral::accept(Visitor &visitor) {
    visitor.visit(*this);
}

} // namespace c::ast