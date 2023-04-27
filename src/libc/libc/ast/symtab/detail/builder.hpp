#pragma once

#include <libc/ast/symtab/symtab.hpp>
#include <libc/ast/visitor.hpp>

namespace c::ast::symtab::detail {

class Builder final : public Visitor {
  public:
    explicit Builder(Symtab &symtab) : symtab_(symtab) {}

    void visit(Program &node);

    void visit(FunctionDefinition &node) override;
    void visit(ast::LocalScope &node) override;

    // Expressions

    void visit(Expression &node) override;
    void visit(FunctionCall &node) override;
    void visit(VariableWriting &node) override;
    void visit(DataCreate &node) override;

    // Statements

    void visit(ReturnStatement &node) override;
    void visit(ForStatement &node) override;
    void visit(IfStatement &node) override;

    // Struct

    // void visit(StructDeclaration &node) override;
    // void visit(StructDefinition &node) override;
    // void visit(StructInit &node) override;
    // void visit(StructUninit &node) override;
    // void visit(StructElementAccess &node) override;
    // void visit(StructType &node) override;

    // Array

    void visit(ArrayUninit &node) override;
    void visit(ArrayElementAccess &node) override;

    // Variable

    void visit(VariableInit &node) override;
    void visit(VariableUninit &node) override;
    void visit(VariableAccess &node) override;

    // Operations

    void visit(Assignment &node) override;
    void visit(RvalueOperation &node) override;
    void visit(PrefixIncrement &node) override;
    void visit(PostfixIncrement &node) override;
    void visit(PrefixDecrement &node) override;
    void visit(PostfixDecrement &node) override;

    // Types

    void visit(ast::ArrayType &node) override;
    void visit(ast::PointerType &node) override;
    void visit(DataType &node) override;
    void visit(BaseType &node) override;
    void visit(VoidType &node) override;

  private:
    // void visit(StructElementRefer & /*node*/) override {}
    void visit(AssignmentOperator & /*node*/) override {}
    void visit(ArithmeticOperator & /*node*/) override {}
    void visit(RelationalOperator & /*node*/) override {}
    void visit(LogicalOperator & /*node*/) override {}
    void visit(HeaderFile & /*node*/) override {}
    void visit(ContinueStatement & /*node*/) override {}
    void visit(BreakStatement & /*node*/) override {}
    void visit(StringLiteral & /*node*/) override {}
    void visit(IntegerLiteral & /*node*/) override {}

    void check_sym_access(const std::string &name) const;
    void check_sym_creation(const std::string &name) const;

    Symtab &symtab_;
    std::stack<Scope *> scopes_;

    std::unique_ptr<Type> type_;
    std::string type_name_;

    std::size_t insertion_order_{0};

    // bool is_struct_symbols_{false};
    // std::string sym_name_;
    // VariableSymbol *sym_{nullptr};
    // bool is_struct_access_{false};
};

} // namespace c::ast::symtab::detail