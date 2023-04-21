// #include <libc/ast/symtab/detail/builder.hpp>
#include "builder.hpp"

namespace c::ast::symtab::detail {

void Builder::visit(Program &node) {
    for (auto *child : node.get_childs()) {
        child->accept(*this);
    }
}

void Builder::visit(FunctionDefinition &node) {
    auto unique_ptr = std::make_unique<FunctionSymbol>(node.id());
    auto *func_sym = unique_ptr.get();
    symtab_.add_sym(std::move(unique_ptr));

    node.return_type()->accept(*this);
    func_sym->set_type(std::move(type_));
    func_sym->set_insertion_order_num(insertion_order_++);
    func_sym->set_number_of_param(node.args_declarations().size());

    std::size_t prev_insertion_order_ = insertion_order_;
    insertion_order_ = 0;
    scopes_.push(func_sym);
    for (auto *arg : node.args_declarations()) {
        arg->accept(*this);
    }
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    scopes_.pop();
    insertion_order_ = prev_insertion_order_;
}

void Builder::visit(ast::LocalScope &node) {
    auto unique_ptr = std::make_unique<LocalScope>();
    auto *local_scope = unique_ptr.get();
    scopes_.top()->nest(std::move(unique_ptr));

    local_scope->set_enclosing_scope(scopes_.top());

    std::size_t prev_insertion_order_ = insertion_order_;
    insertion_order_ = 0;
    scopes_.push(local_scope);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    scopes_.pop();
    insertion_order_ = prev_insertion_order_;
}

// Expressions

void Builder::visit(Expression &node) {
    node.expression()->accept(*this);
}

void Builder::visit(FunctionCall &node) {
    auto *stack_node = symtab_.find_sym(node.id());
    for (; stack_node != nullptr; stack_node = stack_node->prev_) {
        if (dynamic_cast<FunctionSymbol *>(stack_node->sym_.get()) != nullptr) {
            break;
        }
    }
    if (stack_node == nullptr) {
        throw SymtabException("Reference to an undefined symbol " + node.id());
    }

    for (auto *arg : node.args()) {
        arg->accept(*this);
    }
}

void Builder::visit(VariableWriting &node) {
    node.variable_writing()->accept(*this);
}

void Builder::visit(DataCreate &node) {
    node.data_create()->accept(*this);
}

// Statements

void Builder::visit(ReturnStatement &node) {
    node.value()->accept(*this);
}

void Builder::visit(ForStatement &node) {
    auto unique_ptr = std::make_unique<LocalScope>();
    auto *local_scope = unique_ptr.get();
    scopes_.top()->nest(std::move(unique_ptr));

    local_scope->set_enclosing_scope(scopes_.top());

    std::size_t prev_insertion_order_ = insertion_order_;
    insertion_order_ = 0;
    scopes_.push(local_scope);
    node.for_data_using()->accept(*this);
    node.truth_value()->accept(*this);
    node.value()->accept(*this);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    scopes_.pop();
    insertion_order_ = prev_insertion_order_;
}

void Builder::visit(IfStatement &node) {
    auto unique_ptr = std::make_unique<LocalScope>();
    auto *local_scope = unique_ptr.get();
    scopes_.top()->nest(std::move(unique_ptr));

    local_scope->set_enclosing_scope(scopes_.top());

    std::size_t prev_insertion_order_ = insertion_order_;
    insertion_order_ = 0;
    scopes_.push(local_scope);
    node.truth_value()->accept(*this);
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    scopes_.pop();
    insertion_order_ = prev_insertion_order_;
}

// Struct

// Array

void Builder::visit(ArrayUninit &node) {
    check_sym_creation(node.id());

    auto unique_ptr = std::make_unique<VariableSymbol>(node.id());
    auto *var = unique_ptr.get();
    symtab_.add_sym(std::move(unique_ptr));
    scopes_.top()->define(var);

    node.type()->accept(*this);
    var->set_type(std::move(type_));
    var->set_insertion_order_num(insertion_order_++);

    node.size()->accept(*this);
}

void Builder::visit(ArrayElementAccess &node) {
    check_sym_access(node.id());

    node.idx()->accept(*this);
}

// Variable

void Builder::visit(VariableInit &node) {
    check_sym_creation(node.id());

    auto unique_ptr = std::make_unique<VariableSymbol>(node.id());
    auto *var = unique_ptr.get();
    symtab_.add_sym(std::move(unique_ptr));
    scopes_.top()->define(var);

    node.type()->accept(*this);
    var->set_type(std::move(type_));
    var->set_insertion_order_num(insertion_order_++);

    node.value()->accept(*this);
}

void Builder::visit(VariableUninit &node) {
    check_sym_creation(node.id());

    auto unique_ptr = std::make_unique<VariableSymbol>(node.id());
    auto *var = unique_ptr.get();
    symtab_.add_sym(std::move(unique_ptr));
    scopes_.top()->define(var);

    node.type()->accept(*this);
    var->set_type(std::move(type_));
    var->set_insertion_order_num(insertion_order_++);
}

void Builder::visit(VariableAccess &node) {
    check_sym_access(node.id());
}

// Operations

void Builder::visit(Assignment &node) {
    for (auto *child : node.expression()) {
        child->accept(*this);
    }
}

void Builder::visit(RvalueOperation &node) {
    for (auto *child : node.expression()) {
        child->accept(*this);
    }
}

void Builder::visit(PrefixIncrement &node) {
    node.value()->accept(*this);
}

void Builder::visit(PostfixIncrement &node) {
    node.value()->accept(*this);
}

void Builder::visit(PrefixDecrement &node) {
    node.value()->accept(*this);
}

void Builder::visit(PostfixDecrement &node) {
    node.value()->accept(*this);
}

// Types

void Builder::visit(ast::ArrayType &node) {
    node.type()->accept(*this);
    type_ = std::make_unique<ArrayType>(type_name_, node.is_const());
}

void Builder::visit(ast::PointerType &node) {
    node.type()->accept(*this);
    type_ = std::make_unique<symtab::PointerType>(
        type_name_, node.level(), node.is_const());
}

void Builder::visit(DataType &node) {
    node.type()->accept(*this);
    type_ = std::make_unique<PrimitiveType>(type_name_, node.is_const());
}

void Builder::visit(BaseType &node) {
    type_name_ = node.type();
}

void Builder::visit(VoidType &node) {
    type_ = std::make_unique<PrimitiveType>(node.type(), false);
}

void Builder::check_sym_access(const std::string &name) const {
    auto *stack_node = symtab_.find_sym(name);
    if (stack_node == nullptr) {
        throw SymtabException("Reference to an undefined symbol " + name);
    }
    for (auto *cur_scope = scopes_.top(); cur_scope != nullptr;
         cur_scope = cur_scope->get_enclosing_scope()) {
        for (auto *cur_node = stack_node; cur_node != nullptr;
             cur_node = cur_node->prev_) {
            if (cur_scope == cur_node->sym_->get_scope() ||
                nullptr == cur_node->sym_->get_scope()) {
                if (dynamic_cast<VariableSymbol *>(cur_node->sym_.get()) !=
                    nullptr) {
                    return;
                }
            }
        }
    }
    throw SymtabException("Reference to an undefined symbol " + name);
}

void Builder::check_sym_creation(const std::string &name) const {
    auto symbols = scopes_.top()->get_symbols();
    auto it = symbols.find(name);
    if (it != symbols.end()) {
        throw SymtabException(name + " symbol already defined");
    }
}

} // namespace c::ast::symtab::detail