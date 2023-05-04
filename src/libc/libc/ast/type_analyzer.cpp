#include <libc/ast/type_analyzer.hpp>

namespace c::ast {

const std::unordered_map<std::string, std::size_t> c_type_orders = {
    {"char", 0},
    {"short", 1},
    {"int", 2},
    {"long", 3},
    {"float", 4},
    {"double", 5}};

void TypeAnalyzer::exec(Program &program, symtab::Symtab &symtab) {
    TypeAnalyzer type_analyzer(symtab);

    for (auto *child : program.get_childs()) {
        child->accept(type_analyzer);
    }
}

void TypeAnalyzer::visit(FunctionDefinition &node) {
    symtab::FunctionSymbol *func_sym = nullptr;
    for (auto *stack_node = symtab_.find_sym(node.id()); stack_node != nullptr;
         stack_node = stack_node->prev_) {
        if (func_sym =
                dynamic_cast<symtab::FunctionSymbol *>(stack_node->sym_.get());
            func_sym != nullptr) {
            return_type_ = func_sym->get_type();
            scopes_.push(func_sym);
            break;
        }
    }

    for (auto *action : node.actions()) {
        action->accept(*this);
    }

    if (!have_return_ && return_type_->get_name() != "void") {
        throw Exception(node.id() + ": has no return statement");
    }

    have_return_ = false;
    scope_order_ = 0;

    scopes_.pop();
}

void TypeAnalyzer::visit(LocalScope &node) {
    scopes_.push(scopes_.top()->get_nested_scopes()[scope_order_++].get());

    std::size_t prev_scope_order = scope_order_;
    scope_order_ = 0;

    for (auto *action : node.actions()) {
        action->accept(*this);
    }

    scope_order_ = prev_scope_order;

    scopes_.pop();
}

// Expressions

void TypeAnalyzer::visit(Expression &node) {
    node.expression()->accept(*this);
}

void TypeAnalyzer::visit(FunctionCall &node) {
    symtab::FunctionSymbol *func_sym = nullptr;
    for (auto *stack_node = symtab_.find_sym(node.id()); stack_node != nullptr;
         stack_node = stack_node->prev_) {
        if (func_sym =
                dynamic_cast<symtab::FunctionSymbol *>(stack_node->sym_.get());
            func_sym != nullptr) {
            break;
        }
    }

    if (func_sym == nullptr) {
        throw Exception("fail dynamic cast " + node.id());
    }

    if (node.args().size() != func_sym->get_number_of_param()) {
        throw Exception(node.id() + " call: wrong number of parameters");
    }

    const auto &args = node.args();
    auto params = func_sym->get_params();
    for (std::size_t i = 0; i < args.size(); ++i) {
        args[i]->accept(*this);
        auto *param = dynamic_cast<symtab::VariableSymbol *>(params[i]);
        if (param == nullptr) {
            throw Exception(
                node.id() + " call: fail dynamic cast " +
                params[i]->get_name());
        }
        is_possible_type_conversion(type_buf_, param->get_type());
    }

    type_buf_ = func_sym->get_type();
}

void TypeAnalyzer::visit(VariableWriting &node) {
    node.variable_writing()->accept(*this);
}

void TypeAnalyzer::visit(DataCreate &node) {
    node.data_create()->accept(*this);
}

// Statements

void TypeAnalyzer::visit(ReturnStatement &node) {
    if (node.value() == nullptr) {
        if (return_type_->get_name() != "void") {
            throw Exception(
                "return with no value, in function returning non-void");
        }
    } else {
        if (return_type_->get_name() == "void") {
            throw Exception("return with a value, in function returning void");
        }
        node.value()->accept(*this);
        is_possible_type_conversion(type_buf_, return_type_);
    }
    have_return_ = true;
}

void TypeAnalyzer::visit(ForStatement &node) {
    scopes_.push(scopes_.top()->get_nested_scopes()[scope_order_++].get());

    std::size_t prev_scope_order = scope_order_;
    scope_order_ = 0;

    bool is_for_scope_true = for_scope_;

    for_scope_ = true;

    if (node.for_data_using() != nullptr) {
        node.for_data_using()->accept(*this);
    }
    if (node.truth_value() != nullptr) {
        node.truth_value()->accept(*this);
    }
    if (node.value() != nullptr) {
        node.value()->accept(*this);
    }

    for (auto *action : node.actions()) {
        action->accept(*this);
    }

    scope_order_ = prev_scope_order;

    if (!is_for_scope_true) {
        for_scope_ = false;
    }

    scopes_.pop();
}

void TypeAnalyzer::visit(IfStatement &node) {
    scopes_.push(scopes_.top()->get_nested_scopes()[scope_order_++].get());

    std::size_t prev_scope_order = scope_order_;
    scope_order_ = 0;

    node.truth_value()->accept(*this);

    for (auto *action : node.actions()) {
        action->accept(*this);
    }

    scope_order_ = prev_scope_order;

    scopes_.pop();
}

void TypeAnalyzer::visit(ContinueStatement & /*node*/) {
    if (!for_scope_) {
        throw Exception("continue statement isn't in for scope");
    }
}

void TypeAnalyzer::visit(BreakStatement & /*node*/) {
    if (!for_scope_) {
        throw Exception("break statement isn't in for scope");
    }
}

// Array

void TypeAnalyzer::visit(ArrayUninit &node) {
    node.size()->accept(*this);
    valid_array_size_type_check(type_buf_);
}

void TypeAnalyzer::visit(ArrayElementAccess &node) {
    node.idx()->accept(*this);
    valid_array_size_type_check(type_buf_);
    auto *var = dynamic_cast<symtab::VariableSymbol *>(
        scopes_.top()->resolve(node.id()));
    if (var == nullptr) {
        throw Exception("fail dynamic cast " + node.id());
    }
    type_buf_ = var->get_type();
}

// Variable

void TypeAnalyzer::visit(VariableInit &node) {
    node.value()->accept(*this);
    auto *var = dynamic_cast<symtab::VariableSymbol *>(
        scopes_.top()->get_symbol(node.id()));
    if (var == nullptr) {
        throw Exception("fail dynamic cast " + node.id());
    }
    is_possible_type_conversion(type_buf_, var->get_type());
}

void TypeAnalyzer::visit(VariableAccess &node) {
    auto *var = dynamic_cast<symtab::VariableSymbol *>(
        scopes_.top()->resolve(node.id()));
    if (var == nullptr) {
        throw Exception("fail dynamic cast " + node.id());
    }
    type_buf_ = var->get_type();
}

// Operations

void TypeAnalyzer::visit(Assignment &node) {
    node.expression().back()->accept(*this);
    symtab::Type *rhs = type_buf_;
    bool k = true;
    for (auto it = ++node.expression().rbegin(); it != node.expression().rend();
         ++it, k ^= 1) {
        if (k) {
            continue;
        }
        (*it)->accept(*this);
        if (type_buf_->get_type() == std::string("[]") &&
            dynamic_cast<ArrayElementAccess *>(*it) == nullptr) {
            throw Exception("assignment to expression with array type");
        }
        if (rhs->get_name() == "void") {
            throw Exception("assignment to expression with void type");
        }
        // if (type_buf_->get_type() != rhs->get_type()) {
        //     // warning
        // }
        rhs = type_buf_;
    }
}

void TypeAnalyzer::visit(RvalueOperation &node) {
    node.expression().front()->accept(*this);
    symtab::Type *lhs = type_buf_;
    symtab::Type *to = type_buf_;
    std::size_t max_order = c_type_orders.at(type_buf_->get_name());
    bool k = true;
    for (auto it = ++node.expression().begin(); it != node.expression().end();
         ++it, k ^= 1) {
        if (k) {
            continue;
        }
        (*it)->accept(*this);
        if (type_buf_->get_type() != lhs->get_type()) {
            throw Exception(
                "wrong operation between " + type_buf_->get_name() +
                type_buf_->get_type() + " and " + lhs->get_name() +
                lhs->get_type());
        }
        if (type_buf_->get_name() == "void") {
            throw Exception("wrong operation: operand has void type");
        }
        lhs = type_buf_;

        if (c_type_orders.at(type_buf_->get_name()) > max_order) {
            to = type_buf_;
            max_order = c_type_orders.at(type_buf_->get_name());
        }
    }
    type_buf_ = to;
}

void TypeAnalyzer::visit(PrefixIncrement &node) {
    node.value()->accept(*this);
    if (type_buf_->get_type() == std::string("[]")) {
        throw Exception("wrong increment: lvalue has array type");
    }
}

void TypeAnalyzer::visit(PostfixIncrement &node) {
    node.value()->accept(*this);
    if (type_buf_->get_type() == std::string("[]")) {
        throw Exception("wrong increment: lvalue has array type");
    }
}

void TypeAnalyzer::visit(PrefixDecrement &node) {
    node.value()->accept(*this);
    if (type_buf_->get_type() == std::string("[]")) {
        throw Exception("wrong decrement: lvalue has array type");
    }
}

void TypeAnalyzer::visit(PostfixDecrement &node) {
    node.value()->accept(*this);
    if (type_buf_->get_type() == std::string("[]")) {
        throw Exception("wrong decrement: lvalue has array type");
    }
}

// Literals

void TypeAnalyzer::visit(StringLiteral & /*node*/) {
    type_buf_ = &str_l_;
}

void TypeAnalyzer::visit(IntegerLiteral & /*node*/) {
    type_buf_ = &int_l_;
}

void TypeAnalyzer::is_possible_type_conversion(
    symtab::Type *from, symtab::Type * /*to*/) {
    if (from->get_name() == "void") {
        throw Exception("imposible type conversion: operand has void type");
    }
    // if (from->get_type() != to->get_type()) {
    //     // warning
    // }
}

void TypeAnalyzer::valid_array_size_type_check(symtab::Type *type) {
    const auto &type_name = type->get_name();
    if (type->get_type() != std::string("var") ||
        type_name == std::string("float") ||
        type_name == std::string("double")) {
        throw Exception("invalid array size type");
    }
}

} // namespace c::ast