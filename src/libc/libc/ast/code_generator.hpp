#pragma once

#include <libc/ast/symtab/symtab.hpp>
#include <libc/ast/visitor.hpp>

#include <ostream>
#include <stack>
#include <unordered_map>

namespace c::ast {

class CodeGenerator final : public Visitor {
  public:
    struct IrNode {
        IrNode() = default;

        IrNode(std::string name, std::string type)
            : name_(std::move(name)), type_(std::move(type)) {}

        IrNode(std::string name, std::string type, std::string alc_name)
            : name_(std::move(name)), type_(std::move(type)),
              alc_name_(std::move(alc_name)) {}

        IrNode(const IrNode &other) = default;

        IrNode(IrNode &&other) noexcept
            : name_(std::move(other.name_)), type_(std::move(other.type_)),
              alc_name_(std::move(other.alc_name_)) {}

        IrNode &operator=(const IrNode &other) {
            if (this != &other) {
                name_ = other.name_;
                type_ = other.type_;
                alc_name_ = other.alc_name_;
            }
            return *this;
        }

        IrNode &operator=(IrNode &&other) noexcept {
            if (this != &other) {
                name_ = std::move(other.name_);
                type_ = std::move(other.type_);
                alc_name_ = std::move(other.alc_name_);
            }
            return *this;
        }

        std::string name_;
        std::string type_;

        std::string alc_name_;
    };

    CodeGenerator(std::ostream &ir, symtab::Symtab &symtab)
        : symtab_(symtab), ir_(ir) {}

    static void
    exec(std::ostream &os, Program &program, symtab::Symtab &symtab);

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
    void visit(VariableUninit &node) override;
    void visit(VariableAccess &node) override;

    // Operations

    void visit(Assignment &node) override;
    void visit(RvalueOperation &node) override;
    void visit(AssignmentOperator &node) override;
    void visit(ArithmeticOperator &node) override;
    void visit(RelationalOperator &node) override;

    // Literals

    void visit(StringLiteral &node) override;
    void visit(IntegerLiteral &node) override;

  private:
    void visit(HeaderFile & /*node*/) override {}
    void visit(ArrayType & /*node*/) override {}
    void visit(PointerType & /*node*/) override {}
    void visit(DataType & /*node*/) override {}
    void visit(BaseType & /*node*/) override {}
    void visit(VoidType & /*node*/) override {}
    // void visit(LogicalOperator & /*node*/) override {}
    // void visit(PrefixIncrement & /*node*/) override {}
    // void visit(PostfixIncrement & /*node*/) override {}
    // void visit(PrefixDecrement & /*node*/) override {}
    // void visit(PostfixDecrement & /*node*/) override {}

    symtab::FunctionSymbol *get_funcsym(const std::string &id);
    symtab::VariableSymbol *get_varsym(const std::string &id);
    std::string get_ir_type(symtab::Type *type);
    void cast(IrNode &lhs, IrNode &rhs);
    void cast_to(const IrNode &to, IrNode &from);
    void cast_out(
        IrNode &from, const IrNode &to, std::string &ir_name, std::string inst);

    symtab::Symtab &symtab_;

    std::stack<symtab::Scope *> scopes_;
    std::size_t scope_order_{0};

    std::ostream &ir_;

    std::size_t tmp_num_{0};
    std::size_t block_num_{0};
    std::size_t str_num_{0};
    std::size_t addr_num_{0};
    std::vector<std::size_t> loop_nums_;
    std::vector<std::size_t> skip_nums_;

    IrNode ir_buf_;
    std::unordered_map<symtab::Symbol *, IrNode> cgs_alc_;

    std::stack<IrNode> calc_expr_;
    bool is_rvalue_oper_{false};

    bool is_rel_op_last_{false};
};

class DeclareStr final : public Visitor {
  public:
    explicit DeclareStr(std::ostream &os) : ir_(os) {}

    static void exec(std::ostream &os, Program &program);

    void visit(FunctionDefinition &node) override;

    void visit(LocalScope &node) override;

    void visit(Expression &node) override;

    void visit(FunctionCall &node) override;

    void visit(VariableWriting &node) override;

    void visit(DataCreate &node) override;

    void visit(ForStatement &node) override;

    void visit(IfStatement &node) override;

    void visit(VariableInit &node) override;

    void visit(StringLiteral &node) override;

  private:
    void visit(HeaderFile & /*node*/) override {}
    void visit(ReturnStatement & /*node*/) override {}
    void visit(ContinueStatement & /*node*/) override {}
    void visit(BreakStatement & /*node*/) override {}
    void visit(VariableUninit & /*node*/) override {}
    void visit(VariableAccess & /*node*/) override {}
    void visit(ArrayUninit & /*node*/) override {}
    void visit(ArrayElementAccess & /*node*/) override {}
    void visit(Assignment & /*node*/) override {}
    void visit(RvalueOperation & /*node*/) override {}
    void visit(AssignmentOperator & /*node*/) override {}
    void visit(ArithmeticOperator & /*node*/) override {}
    void visit(RelationalOperator & /*node*/) override {}
    // void visit(LogicalOperator & /*node*/) override {}
    // void visit(PrefixIncrement & /*node*/) override {}
    // void visit(PostfixIncrement & /*node*/) override {}
    // void visit(PrefixDecrement & /*node*/) override {}
    // void visit(PostfixDecrement & /*node*/) override {}
    void visit(ArrayType & /*node*/) override {}
    void visit(PointerType & /*node*/) override {}
    void visit(DataType & /*node*/) override {}
    void visit(BaseType & /*node*/) override {}
    void visit(VoidType & /*node*/) override {}
    void visit(IntegerLiteral & /*node*/) override {}

    std::ostream &ir_;
    std::size_t str_num_{0};
    bool is_printf_exist_{false};
};

} // namespace c::ast