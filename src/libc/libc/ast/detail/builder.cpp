#include <libc/ast/detail/builder.hpp>

namespace c::ast::detail {

std::any Builder::visitProgram(CParser::ProgramContext *context) {
    Childs childs;
    for (auto *element : context->element()) {
        childs.push_back(std::any_cast<Node *>(visitChildren(element)));
    }
    program_.set_childs(childs);
    return childs;
}

static std::string trim_quotes(const std::string &str) {
    assert(str[0] == '"' && str[str.size() - 1] == '"');
    return str.substr(1, str.size() - 2);
}

std::any Builder::visitHeader_file(CParser::Header_fileContext *context) {
    auto header_file = context->HEADER_FILE()->getText();
    assert(header_file[0] == '<' && header_file[header_file.size() - 1] == '>');
    return static_cast<Node *>(program_.create_node<HeaderFile>(
        header_file.substr(1, header_file.size() - 2)));
}

std::any Builder::visitFunction_definition(
    CParser::Function_definitionContext *context) {
    const auto return_type = context->return_type()->getText();
    visit(context->return_type());

    const auto id = context->ID()->getText();

    Childs childs;
    for (auto *child : context->action()) {
        childs.push_back(
            std::any_cast<Node *>(visit(child->children[0]->children[0])));
    }

    return static_cast<Node *>(
        program_.create_node<FunctionDefinition>(return_type, id, childs));
}

std::any Builder::visitFunction_call(CParser::Function_callContext *context) {
    const auto id = context->ID()->getText();

    Childs args;
    for (auto *arg : context->arg()) {
        args.push_back(std::any_cast<Node *>(visit(arg)));
    }

    return static_cast<Node *>(program_.create_node<FunctionCall>(id, args));
}

std::any Builder::visitArg(CParser::ArgContext *context) {
    Node *arg = nullptr;
    arg = std::any_cast<Node *>(visit(context->children[0]));
    return static_cast<Node *>(program_.create_node<Arg>(arg));
}

std::any
Builder::visitReturn_statement(CParser::Return_statementContext *context) {
    Node *literal = nullptr;
    literal = std::any_cast<Node *>(visit(context->integer_literal()));
    return static_cast<Node *>(program_.create_node<ReturnStatement>(literal));
}

std::any Builder::visitString_literal(CParser::String_literalContext *context) {
    const auto string = trim_quotes(context->STRING()->getText());
    return static_cast<Node *>(program_.create_node<StringLiteral>(string));
}

std::any
Builder::visitInteger_literal(CParser::Integer_literalContext *context) {
    const auto integer = context->INTEGER()->getText();
    return static_cast<Node *>(program_.create_node<IntegerLiteral>(integer));
}

} // namespace c::ast::detail