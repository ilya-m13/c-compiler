#include <libc/ast/detail/builder.hpp>
#include <libc/ast/detail/precedence_builder.hpp>

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
    auto *return_type = std::any_cast<Node *>(visit(context->return_type()));

    const auto id = context->ID()->getText();

    Childs args_declarations;
    for (auto *args_declaration : context->arg_declaration()) {
        args_declarations.push_back(
            std::any_cast<Node *>(visit(args_declaration)));
    }

    Childs actions;
    for (auto *action : context->action()) {
        actions.push_back(std::any_cast<Node *>(visit(action)));
    }

    return static_cast<Node *>(program_.create_node<FunctionDefinition>(
        return_type, id, actions, args_declarations));
}

std::any Builder::visitLocal_scope(CParser::Local_scopeContext *context) {
    Childs actions;
    for (auto *action : context->action()) {
        actions.push_back(std::any_cast<Node *>(visit(action)));
    }

    return static_cast<Node *>(program_.create_node<LocalScope>(actions));
}

// Expressions

std::any Builder::visitExpression(CParser::ExpressionContext *context) {
    auto *expression = std::any_cast<Node *>(visit(context->children[0]));
    return static_cast<Node *>(program_.create_node<Expression>(expression));
}

std::any Builder::visitFunction_call(CParser::Function_callContext *context) {
    const auto id = context->ID()->getText();

    Childs args;
    for (auto *arg : context->arg()) {
        args.push_back(std::any_cast<Node *>(visit(arg)));
    }

    return static_cast<Node *>(program_.create_node<FunctionCall>(id, args));
}

std::any
Builder::visitVariable_writing(CParser::Variable_writingContext *context) {
    auto *variable_writing = std::any_cast<Node *>(visit(context->children[0]));

    return static_cast<Node *>(
        program_.create_node<VariableWriting>(variable_writing));
}
std::any Builder::visitData_create(CParser::Data_createContext *context) {
    auto *data_create = std::any_cast<Node *>(visit(context->children[0]));

    return static_cast<Node *>(program_.create_node<DataCreate>(data_create));
}

// Statements

std::any
Builder::visitReturn_statement(CParser::Return_statementContext *context) {
    auto *value = std::any_cast<Node *>(visit(context->value()));
    return static_cast<Node *>(program_.create_node<ReturnStatement>(value));
}

std::any Builder::visitFor_statement(CParser::For_statementContext *context) {
    Node *for_data_using = nullptr;
    if (context->for_data_using() != nullptr) {
        for_data_using =
            std::any_cast<Node *>(visit(context->for_data_using()));
    }

    Node *truth_value = nullptr;
    if (context->truth_value() != nullptr) {
        truth_value = std::any_cast<Node *>(visit(context->truth_value()));
    }

    Node *value = nullptr;
    if (context->value() != nullptr) {
        value = std::any_cast<Node *>(visit(context->value()));
    }

    Childs actions;
    for (auto *action : context->action()) {
        actions.push_back(std::any_cast<Node *>(visit(action)));
    }

    return static_cast<Node *>(program_.create_node<ForStatement>(
        for_data_using, truth_value, value, actions));
}

std::any Builder::visitIf_statement(CParser::If_statementContext *context) {
    auto *truth_value = std::any_cast<Node *>(visit(context->truth_value()));

    Childs actions;
    for (auto *action : context->action()) {
        actions.push_back(std::any_cast<Node *>(visit(action)));
    }

    return static_cast<Node *>(
        program_.create_node<IfStatement>(truth_value, actions));
}

std::any Builder::visitContinue_statement(
    CParser::Continue_statementContext * /*context*/) {
    return static_cast<Node *>(program_.create_node<ContinueStatement>());
}

std::any
Builder::visitBreak_statement(CParser::Break_statementContext * /*context*/) {
    return static_cast<Node *>(program_.create_node<BreakStatement>());
}

// Struct

// TODO: equal ID[0] with ID[1]
std::any
Builder::visitStruct_declaration(CParser::Struct_declarationContext *context) {
    auto id = context->ID()->getText();

    return static_cast<Node *>(program_.create_node<StructDeclaration>(id));
}

std::any
Builder::visitStruct_definition(CParser::Struct_definitionContext *context) {
    auto id = context->ID()[0]->getText();
    std::string object;
    if (context->ID().size() != 1) {
        object = context->ID()[1]->getText();
    }
    Childs data_uninit;
    for (auto *child : context->data_uninit()) {
        data_uninit.push_back(std::any_cast<Node *>(visit(child)));
    }

    return static_cast<Node *>(
        program_.create_node<StructDefinition>(id, object, data_uninit));
}

std::any Builder::visitStruct_init(CParser::Struct_initContext *context) {
    auto *struct_type = std::any_cast<Node *>(visit(context->struct_type()));
    auto id = context->ID()->getText();
    Childs values;
    for (auto *value : context->value()) {
        values.push_back(std::any_cast<Node *>(visit(value)));
    }

    return static_cast<Node *>(
        program_.create_node<StructInit>(struct_type, id, values));
}

std::any Builder::visitStruct_uninit(CParser::Struct_uninitContext *context) {
    auto *struct_type = std::any_cast<Node *>(visit(context->struct_type()));
    auto id = context->ID()->getText();

    return static_cast<Node *>(
        program_.create_node<StructUninit>(struct_type, id));
}

std::any Builder::visitStruct_element_access(
    CParser::Struct_element_accessContext *context) {
    Childs lvalue_refer_stream;
    for (auto *child : context->children) {
        lvalue_refer_stream.push_back(std::any_cast<Node *>(visit(child)));
    }

    return static_cast<Node *>(
        program_.create_node<StructElementAccess>(lvalue_refer_stream));
}

std::any Builder::visitStruct_type(CParser::Struct_typeContext *context) {
    bool is_const = static_cast<bool>(context->CONST());
    auto id = context->ID()->getText();

    return static_cast<Node *>(program_.create_node<StructType>(is_const, id));
}

std::any Builder::visitStruct_element_refer(
    CParser::Struct_element_referContext *context) {
    auto element_refer = context->children[0]->getText();

    return static_cast<Node *>(
        program_.create_node<StructElementRefer>(element_refer));
}

// Array

std::any Builder::visitArray_uninit(CParser::Array_uninitContext *context) {
    auto *type = std::any_cast<Node *>(visit(context->data_type()));
    auto id = context->ID()->getText();
    auto *size = std::any_cast<Node *>(visit(context->value()));

    return static_cast<Node *>(
        program_.create_node<ArrayUninit>(type, id, size));
}

std::any Builder::visitArray_element_access(
    CParser::Array_element_accessContext *context) {
    auto id = context->ID()->getText();
    auto *idx = std::any_cast<Node *>(visit(context->value()));

    return static_cast<Node *>(
        program_.create_node<ArrayElementAccess>(id, idx));
}

// Variable

std::any Builder::visitVariable_init(CParser::Variable_initContext *context) {
    auto *type = std::any_cast<Node *>(visit(context->variable_type()));
    auto id = context->ID()->getText();
    auto *value = std::any_cast<Node *>(visit(context->value()));

    return static_cast<Node *>(
        program_.create_node<VariableInit>(type, id, value));
}

std::any
Builder::visitVariable_uninit(CParser::Variable_uninitContext *context) {
    auto *type = std::any_cast<Node *>(visit(context->variable_type()));
    auto id = context->ID()->getText();

    return static_cast<Node *>(program_.create_node<VariableUninit>(type, id));
}

std::any
Builder::visitVariable_access(CParser::Variable_accessContext *context) {
    auto id = context->ID()->getText();

    return static_cast<Node *>(program_.create_node<VariableAccess>(id));
}

// Operations

std::any Builder::visitAssignment(CParser::AssignmentContext *context) {
    Childs expression;
    for (auto *child : context->children) {
        expression.push_back(std::any_cast<Node *>(visit(child)));
    }

    Childs rpn = PrecedenceBuilder::exec(expression);

    return static_cast<Node *>(
        program_.create_node<Assignment>(expression, rpn));
}

std::any
Builder::visitRvalue_operation(CParser::Rvalue_operationContext *context) {
    Childs expression;
    for (auto *child : context->children) {
        expression.push_back(std::any_cast<Node *>(visit(child)));
    }

    Childs rpn = PrecedenceBuilder::exec(expression);

    return static_cast<Node *>(
        program_.create_node<RvalueOperation>(expression, rpn));
}

std::any Builder::visitAssignment_operator(
    CParser::Assignment_operatorContext *context) {
    auto assign_operator = context->children[0]->getText();

    return static_cast<Node *>(
        program_.create_node<AssignmentOperator>(assign_operator));
}

std::any Builder::visitArithmetic_operator(
    CParser::Arithmetic_operatorContext *context) {
    auto arithmetic_operator = context->children[0]->getText();

    return static_cast<Node *>(
        program_.create_node<ArithmeticOperator>(arithmetic_operator));
}

std::any Builder::visitRelational_operator(
    CParser::Relational_operatorContext *context) {
    auto relational_operator = context->children[0]->getText();

    return static_cast<Node *>(
        program_.create_node<RelationalOperator>(relational_operator));
}

std::any
Builder::visitLogical_operator(CParser::Logical_operatorContext *context) {
    auto logical_operator = context->children[0]->getText();

    return static_cast<Node *>(
        program_.create_node<LogicalOperator>(logical_operator));
}

std::any
Builder::visitPrefix_increment(CParser::Prefix_incrementContext *context) {
    auto *value = std::any_cast<Node *>(visit(context->lvalue()));

    return static_cast<Node *>(program_.create_node<PrefixIncrement>(value));
}

std::any
Builder::visitPostfix_increment(CParser::Postfix_incrementContext *context) {
    auto *value = std::any_cast<Node *>(visit(context->lvalue()));

    return static_cast<Node *>(program_.create_node<PostfixIncrement>(value));
}

std::any
Builder::visitPrefix_decrement(CParser::Prefix_decrementContext *context) {
    auto *value = std::any_cast<Node *>(visit(context->lvalue()));

    return static_cast<Node *>(program_.create_node<PrefixDecrement>(value));
}

std::any
Builder::visitPostfix_decrement(CParser::Postfix_decrementContext *context) {
    auto *value = std::any_cast<Node *>(visit(context->lvalue()));

    return static_cast<Node *>(program_.create_node<PostfixDecrement>(value));
}

// Types

std::any Builder::visitPointer_type(CParser::Pointer_typeContext *context) {
    bool is_const = static_cast<bool>(context->CONST());
    Node *sign = nullptr;
    if (context->sign() != nullptr) {
        sign = std::any_cast<Node *>(visit(context->sign()));
    }
    auto *type = std::any_cast<Node *>(visit(context->any_type()));
    std::size_t level = context->MULTIP().size();

    return static_cast<Node *>(
        program_.create_node<PointerType>(is_const, sign, type, level));
}

std::any Builder::visitData_type(CParser::Data_typeContext *context) {
    bool is_const = static_cast<bool>(context->CONST());
    Node *sign = nullptr;
    if (context->sign() != nullptr) {
        sign = std::any_cast<Node *>(visit(context->sign()));
    }
    auto *type = std::any_cast<Node *>(visit(context->base_type()));

    return static_cast<Node *>(
        program_.create_node<DataType>(is_const, sign, type));
}

std::any Builder::visitBase_type(CParser::Base_typeContext *context) {
    auto type = context->children[0]->getText();

    return static_cast<Node *>(program_.create_node<BaseType>(type));
}

std::any Builder::visitVoid_type(CParser::Void_typeContext * /*context*/) {
    return static_cast<Node *>(program_.create_node<VoidType>());
}

std::any Builder::visitSign(CParser::SignContext *context) {
    auto sign = context->children[0]->getText();

    return static_cast<Node *>(program_.create_node<Sign>(sign));
}

// Literals

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