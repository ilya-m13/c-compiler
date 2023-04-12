#pragma once

#include <libc/ast/visitor.hpp>

#include <pugixml.hpp>

#include <ostream>
#include <stack>

namespace c::ast {

class XmlSerializer final : public Visitor {
  public:
    static void exec(Program &program, std::ostream &out);

    void visit(HeaderFile &node) override;
    void visit(FunctionDefinition &node) override;

    // Expressions

    void visit(Expression &node) override;
    void visit(FunctionCall &node) override;
    void visit(VariableWriting &node) override;
    void visit(DataCreate &node) override;

    // Statements

    void visit(ReturnStatement &node) override;
    void visit(ForStatement &node) override;
    void visit(IfStatement &node) override;
    void visit(ContinueStatement &node) override;
    void visit(BreakStatement &node) override;

    // Struct

    void visit(StructDeclaration &node) override;
    void visit(StructDefinition &node) override;
    void visit(StructInit &node) override;
    void visit(StructUninit &node) override;
    void visit(StructElementAccess &node) override;
    void visit(StructType &node) override;
    void visit(StructElementRefer &node) override;

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
    void visit(LogicalOperator &node) override;
    void visit(PrefixIncrement &node) override;
    void visit(PostfixIncrement &node) override;
    void visit(PrefixDecrement &node) override;
    void visit(PostfixDecrement &node) override;

    // Types

    void visit(PointerType &node) override;
    void visit(DataType &node) override;
    void visit(BaseType &node) override;
    void visit(VoidType &node) override;
    void visit(Sign &node) override;

    // Literals

    void visit(StringLiteral &node) override;
    void visit(IntegerLiteral &node) override;

  private:
    pugi::xml_node append_child(const char *name);
    void append_text(const char *text);
    void append_attribute(const char *name, const char *value);

    pugi::xml_document doc_;
    std::stack<pugi::xml_node> nodes_;
};

} // namespace c::ast