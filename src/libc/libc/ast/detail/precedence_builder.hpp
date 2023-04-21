#pragma once

#include <libc/ast/visitor.hpp>

#include <stack>

namespace c::ast::detail {

struct OperatorData;

class PrecedenceBuilder final : public Visitor {
  public:
    static Childs exec(const Childs &expression);

    void visit(RvalueOperation &node) override;

    void visit(AssignmentOperator &node) override;
    void visit(ArithmeticOperator &node) override;
    void visit(RelationalOperator &node) override;
    void visit(LogicalOperator &node) override;

    void visit(PrefixIncrement &node) override;
    void visit(PostfixIncrement &node) override;
    void visit(PrefixDecrement &node) override;
    void visit(PostfixDecrement &node) override;

    void visit(FunctionCall &node) override;
    void visit(ArrayElementAccess &node) override;
    void visit(StructElementAccess &node) override;
    void visit(VariableAccess &node) override;

    void visit(StringLiteral &node) override;
    void visit(IntegerLiteral &node) override;

  private:
    void visit(HeaderFile & /*node*/) override {}
    void visit(FunctionDefinition & /*node*/) override {}
    void visit(LocalScope & /*node*/) override {}
    void visit(Expression & /*node*/) override {}
    void visit(VariableWriting & /*node*/) override {}
    void visit(DataCreate & /*node*/) override {}
    void visit(ReturnStatement & /*node*/) override {}
    void visit(ForStatement & /*node*/) override {}
    void visit(IfStatement & /*node*/) override {}
    void visit(ContinueStatement & /*node*/) override {}
    void visit(BreakStatement & /*node*/) override {}
    void visit(StructDeclaration & /*node*/) override {}
    void visit(StructDefinition & /*node*/) override {}
    void visit(StructInit & /*node*/) override {}
    void visit(StructUninit & /*node*/) override {}
    void visit(StructType & /*node*/) override {}
    void visit(StructElementRefer & /*node*/) override {}
    void visit(ArrayUninit & /*node*/) override {}
    void visit(VariableInit & /*node*/) override {}
    void visit(VariableUninit & /*node*/) override {}
    void visit(Assignment & /*node*/) override {}
    void visit(ArrayType & /*node*/) override {}
    void visit(PointerType & /*node*/) override {}
    void visit(DataType & /*node*/) override {}
    void visit(BaseType & /*node*/) override {}
    void visit(VoidType & /*node*/) override {}

    void handle_operator(const OperatorData &operator_data);

    std::stack<OperatorData> operator_data_;
    Childs rpn_;
};

} // namespace c::ast::detail