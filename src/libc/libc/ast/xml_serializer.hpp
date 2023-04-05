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

    void visit(FunctionCall &node) override;
    void visit(Arg &node) override;

    void visit(ReturnStatement &node) override;

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