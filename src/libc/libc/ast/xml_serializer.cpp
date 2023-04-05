#include <libc/ast/xml_serializer.hpp>

namespace c::ast {

void XmlSerializer::exec(Program &program, std::ostream &out) {
    XmlSerializer xml_serializer;
    xml_serializer.nodes_.push(xml_serializer.doc_.append_child("Program"));
    for (auto *child : program.get_childs()) {
        child->accept(xml_serializer);
    }
    xml_serializer.doc_.save(out);
}

void XmlSerializer::visit(HeaderFile &node) {
    auto header_file = append_child("header-file");
    nodes_.push(header_file);
    append_text(node.file_name().c_str());
    nodes_.pop();
}

void XmlSerializer::visit(FunctionDefinition &node) {
    auto function_definition = append_child("function");
    nodes_.push(function_definition);
    append_attribute("return-type", node.return_type().c_str());
    append_attribute("name", node.id().c_str());
    for (auto *child : node.actions()) {
        child->accept(*this);
    }
    nodes_.pop();
}

void XmlSerializer::visit(FunctionCall &node) {
    auto function_call = append_child("call");
    nodes_.push(function_call);
    append_attribute("name", node.id().c_str());
    for (auto *arg : node.args()) {
        arg->accept(*this);
    }
    nodes_.pop();
}

void XmlSerializer::visit(Arg &node) {
    auto arg = append_child("arg");
    nodes_.push(arg);
    node.arg()->accept(*this);
    nodes_.pop();
}

void XmlSerializer::visit(ReturnStatement &node) {
    auto return_statement = append_child("return");
    nodes_.push(return_statement);
    node.literal()->accept(*this);
    nodes_.pop();
}

void XmlSerializer::visit(StringLiteral &node) {
    append_text(('"' + node.string() + '"').c_str());
}

void XmlSerializer::visit(IntegerLiteral &node) {
    append_text(node.integer().c_str());
}

pugi::xml_node XmlSerializer::append_child(const char *name) {
    return nodes_.top().append_child(name);
}

void XmlSerializer::append_text(const char *text) {
    nodes_.top().append_child(pugi::node_pcdata).set_value(text);
}

void XmlSerializer::append_attribute(const char *name, const char *value) {
    nodes_.top().append_attribute(name).set_value(value);
}
} // namespace c::ast