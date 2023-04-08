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
    node.return_type()->accept(*this);
    append_attribute("name", node.id().c_str());
    for (auto *args_declaration : node.args_declarations()) {
        auto arg_declaration = append_child("arg");
        nodes_.push(arg_declaration);
        args_declaration->accept(*this);
        nodes_.pop();
    }
    auto actions_node = append_child("actions");
    nodes_.push(actions_node);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    nodes_.pop();
    nodes_.pop();
}

// Expressions

void XmlSerializer::visit(Expression &node) {
    node.expression()->accept(*this);
}

void XmlSerializer::visit(FunctionCall &node) {
    auto function_call = append_child("call");
    nodes_.push(function_call);
    append_attribute("name", node.id().c_str());
    for (auto *arg : node.args()) {
        auto arg_node = append_child("arg");
        nodes_.push(arg_node);
        arg->accept(*this);
        nodes_.pop();
    }
    nodes_.pop();
}

void XmlSerializer::visit(VariableWriting &node) {
    auto variable_writing_node = append_child("variable-writing");
    nodes_.push(variable_writing_node);
    node.variable_writing()->accept(*this);
    nodes_.pop();
}

void XmlSerializer::visit(DataCreate &node) {
    auto data_create_node = append_child("data-create");
    nodes_.push(data_create_node);
    node.data_create()->accept(*this);
    nodes_.pop();
}

// Statements

void XmlSerializer::visit(ReturnStatement &node) {
    auto return_statement = append_child("return");
    nodes_.push(return_statement);
    node.value()->accept(*this);
    nodes_.pop();
}

void XmlSerializer::visit(ForStatement &node) {
    auto for_statement = append_child("for");
    nodes_.push(for_statement);
    if (node.for_data_using() != nullptr) {
        node.for_data_using()->accept(*this);
    }
    if (node.truth_value() != nullptr) {
        auto truth_value_node = append_child("truth-value");
        nodes_.push(truth_value_node);
        node.truth_value()->accept(*this);
        nodes_.pop();
    }
    if (node.value() != nullptr) {
        auto value_node = append_child("value");
        nodes_.push(value_node);
        node.value()->accept(*this);
        nodes_.pop();
    }
    auto actions_node = append_child("actions");
    nodes_.push(actions_node);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    nodes_.pop();
    nodes_.pop();
}

void XmlSerializer::visit(IfStatement &node) {
    auto if_node = append_child("if");
    nodes_.push(if_node);

    auto truth_value_node = append_child("truth-value");
    nodes_.push(truth_value_node);
    node.truth_value()->accept(*this);
    nodes_.pop();

    auto actions_node = append_child("actions");
    nodes_.push(actions_node);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    nodes_.pop();

    nodes_.pop();
}

void XmlSerializer::visit(ContinueStatement & /*node*/) {
    append_child("continue");
}

void XmlSerializer::visit(BreakStatement & /*node*/) {
    append_child("break");
}

// Struct

void XmlSerializer::visit(StructDeclaration &node) {
    auto struct_declaration_node = append_child("struct-declaration");
    nodes_.push(struct_declaration_node);
    if (node.is_typedef()) {
        append_attribute("name", node.id().c_str());
    } else {
        append_attribute("name", ("struct-" + node.id()).c_str());
    }
    nodes_.pop();
}

void XmlSerializer::visit(StructDefinition &node) {
    auto struct_definition_node = append_child("struct-definition");
    nodes_.push(struct_definition_node);

    if (node.is_typedef()) {
        append_attribute("name", node.id().c_str());
    } else {
        append_attribute("name", ("struct-" + node.id()).c_str());
    }

    if (!node.object().empty()) {
        append_attribute("object", node.object().c_str());
    }

    for (auto *child : node.data_uninit()) {
        auto member_node = append_child("member");
        nodes_.push(member_node);
        child->accept(*this);
        nodes_.pop();
    }

    nodes_.pop();
}

void XmlSerializer::visit(StructInit &node) {
    node.struct_type()->accept(*this);
    append_attribute("name", node.id().c_str());

    for (auto *value : node.values()) {
        auto value_node = append_child("value");
        nodes_.push(value_node);
        value->accept(*this);
        nodes_.pop();
    }
}

void XmlSerializer::visit(StructUninit &node) {
    node.struct_type()->accept(*this);
    append_attribute("name", node.id().c_str());
}

void XmlSerializer::visit(StructElementAccess &node) {
    for (auto *child : node.lvalue_refer_stream()) {
        child->accept(*this);
    }
}

void XmlSerializer::visit(StructType &node) {
    if (node.is_const()) {
        append_attribute("const", "true");
    }
    append_attribute("type", node.id().c_str());
}

void XmlSerializer::visit(StructElementRefer &node) {
    append_text(node.element_refer().c_str());
}

// Array

void XmlSerializer::visit(ArrayUninit &node) {
    if (node.sign() != nullptr) {
        node.sign()->accept(*this);
    }
    node.type()->accept(*this);
    append_attribute("name", node.id().c_str());

    auto size_node = append_child("size");
    nodes_.push(size_node);
    node.size()->accept(*this);
    nodes_.pop();
}

void XmlSerializer::visit(ArrayElementAccess &node) {
    append_text(node.id().c_str());
    append_text("[");
    node.idx()->accept(*this);
    append_text("]");
}

// Variable

void XmlSerializer::visit(VariableInit &node) {
    node.type()->accept(*this);
    append_attribute("name", node.id().c_str());
    node.value()->accept(*this);
}

void XmlSerializer::visit(VariableUninit &node) {
    node.type()->accept(*this);
    append_attribute("name", node.id().c_str());
}

void XmlSerializer::visit(VariableAccess &node) {
    append_text(node.id().c_str());
}

// Operations

void XmlSerializer::visit(Assignment &node) {
    // for (auto *child : node.expression()) {
    //     child->accept(*this);
    // }
    for (auto *child : node.rpn()) {
        child->accept(*this);
    }
}

void XmlSerializer::visit(RvalueOperation &node) {
    // for (auto *child : node.expression()) {
    //     child->accept(*this);
    // }
    for (auto *child : node.rpn()) {
        child->accept(*this);
    }
}

void XmlSerializer::visit(AssignmentOperator &node) {
    append_text(node.assign_operator().c_str());
}

void XmlSerializer::visit(ArithmeticOperator &node) {
    append_text(node.arithmetic_operator().c_str());
}

void XmlSerializer::visit(RelationalOperator &node) {
    append_text(node.relational_operator().c_str());
}

void XmlSerializer::visit(LogicalOperator &node) {
    append_text(node.logical_operator().c_str());
}

void XmlSerializer::visit(PrefixIncrement &node) {
    append_text("++");
    node.value()->accept(*this);
}

void XmlSerializer::visit(PostfixIncrement &node) {
    node.value()->accept(*this);
    append_text("++");
}

void XmlSerializer::visit(PrefixDecrement &node) {
    append_text("--");
    node.value()->accept(*this);
}

void XmlSerializer::visit(PostfixDecrement &node) {
    node.value()->accept(*this);
    append_text("--");
}

// Types

void XmlSerializer::visit(PointerType &node) {
    if (node.is_const()) {
        append_attribute("const", "true");
    }
    if (node.sign() != nullptr) {
        node.sign()->accept(*this);
    }
    node.type()->accept(*this);

    std::string str;
    for (std::size_t i = 0; i < node.level(); ++i) {
        str.push_back('*');
    }
    append_attribute("level", str.c_str());
}

void XmlSerializer::visit(DataType &node) {
    if (node.is_const()) {
        append_attribute("const", "true");
    }
    if (node.sign() != nullptr) {
        node.sign()->accept(*this);
    }
    node.type()->accept(*this);
}

void XmlSerializer::visit(BaseType &node) {
    append_attribute("type", node.type().c_str());
}

void XmlSerializer::visit(VoidType &node) {
    append_attribute("type", node.type());
}

void XmlSerializer::visit(Sign &node) {
    append_attribute("sign", node.sign().c_str());
}

// Literals

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