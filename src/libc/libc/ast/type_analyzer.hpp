#pragma once

#include <libc/ast/symtab/symtab.hpp>
#include <libc/ast/visitor.hpp>

namespace c::ast {

class TypeAnalyzer : public Visitor {
  public:
    class Exception : public std::runtime_error {
      public:
        using std::runtime_error::runtime_error;
    };

    explicit TypeAnalyzer(symtab::Symtab &symtab) : symtab_(symtab) {}

    static void exec(Program &program, symtab::Symtab &symtab);

    void visit(FunctionDefinition &node) override;
    void visit(LocalScope &node) override;

    // Expressions

    void visit(Expression &node) override;
    void visit(FunctionCall &node) override;
    void visit(VariableWriting &node) override;
    void visit(DataCreate &node) override;

    // Statements

    void visit(ReturnStatement &node) override;
    void visit(ForStatement &node) override;
    void visit(IfStatement &node) override;
    void visit(ContinueStatement & /*node*/) override;
    void visit(BreakStatement & /*node*/) override;

    // Array

    void visit(ArrayUninit &node) override;
    void visit(ArrayElementAccess &node) override;

    // Variable

    void visit(VariableInit &node) override;
    void visit(VariableAccess &node) override;

    // Operations

    void visit(Assignment &node) override;
    void visit(RvalueOperation &node) override;
    void visit(PrefixIncrement &node) override;
    void visit(PostfixIncrement &node) override;
    void visit(PrefixDecrement &node) override;
    void visit(PostfixDecrement &node) override;

    // Literals

    void visit(StringLiteral & /*node*/) override;
    void visit(IntegerLiteral & /*node*/) override;

  private:
    void visit(HeaderFile & /*node*/) override {}
    void visit(VariableUninit & /*node*/) override {}
    void visit(AssignmentOperator & /*node*/) override {}
    void visit(ArithmeticOperator & /*node*/) override {}
    void visit(RelationalOperator & /*node*/) override {}
    void visit(LogicalOperator & /*node*/) override {}
    void visit(ArrayType & /*node*/) override {}
    void visit(PointerType & /*node*/) override {}
    void visit(DataType & /*node*/) override {}
    void visit(BaseType & /*node*/) override {}
    void visit(VoidType & /*node*/) override {}

    void is_possible_type_conversion(symtab::Type *from, symtab::Type * /*to*/);
    void valid_array_size_type_check(symtab::Type *type);

    symtab::Symtab &symtab_;

    std::stack<symtab::Scope *> scopes_;
    std::size_t scope_order_{0};

    symtab::Type *return_type_{nullptr};
    bool have_return_{false};

    symtab::Type *type_buf_{nullptr};
    symtab::PointerType str_l_{"char", 1, false};
    symtab::PrimitiveType int_l_{"int", false};

    bool for_scope_{false};
};

} // namespace c::ast