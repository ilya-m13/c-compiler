#include <libc/ast/detail/precedence_builder.hpp>

namespace c::ast::detail {

namespace {

struct Operators {
    std::vector<std::string> operators_;
    bool is_left_associativity_ = false;
};

// Operators are ordered from highest precedence to lowest
const std::vector<Operators> precedence = {
    {{"*", "/", "%"}, true},
    {{"+", "-"}, true},
    {{"<", "<=", ">", ">="}, true},
    {{"==", "!="}, true},
    {{"&&"}, true},
    {{"||"}, true},
    {{"=", "+=", "-=", "*=", "/=", "%="}, false}};

} // namespace

struct OperatorData {
    static OperatorData
    give_precedence(Node *operator_node, const std::string &operator_str) {
        OperatorData operator_data;
        operator_data.precedence_ = 0;
        for (const auto &operators : precedence) {
            for (const auto &op : operators.operators_) {
                if (op == operator_str) {
                    operator_data.is_left_associativity_ =
                        operators.is_left_associativity_;
                    operator_data.operator_node_ = operator_node;
                    return operator_data;
                }
            }
            ++operator_data.precedence_;
        }
        return operator_data;
    }

    Node *operator_node_ = nullptr;
    std::size_t precedence_ = 0;
    bool is_left_associativity_ = false;
};

Childs PrecedenceBuilder::exec(const Childs &expression) {
    PrecedenceBuilder precedence_builder;
    for (auto *child : expression) {
        child->accept(precedence_builder);
    }
    while (!precedence_builder.operator_data_.empty()) {
        precedence_builder.rpn_.push_back(
            precedence_builder.operator_data_.top().operator_node_);
        precedence_builder.operator_data_.pop();
    }
    return precedence_builder.rpn_;
}

void PrecedenceBuilder::visit(RvalueOperation &node) {
    for (auto *child : node.expression()) {
        child->accept(*this);
    }
}

void PrecedenceBuilder::visit(AssignmentOperator &node) {
    auto operator_data =
        OperatorData::give_precedence(&node, node.assign_operator());
    handle_operator(operator_data);
}

void PrecedenceBuilder::visit(ArithmeticOperator &node) {
    auto operator_data =
        OperatorData::give_precedence(&node, node.arithmetic_operator());
    handle_operator(operator_data);
}

void PrecedenceBuilder::visit(RelationalOperator &node) {
    auto operator_data =
        OperatorData::give_precedence(&node, node.relational_operator());
    handle_operator(operator_data);
}

void PrecedenceBuilder::visit(LogicalOperator &node) {
    auto operator_data =
        OperatorData::give_precedence(&node, node.logical_operator());
    handle_operator(operator_data);
}

void PrecedenceBuilder::visit(PrefixIncrement &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(PostfixIncrement &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(PrefixDecrement &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(PostfixDecrement &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(FunctionCall &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(ArrayElementAccess &node) {
    rpn_.push_back(&node);
}

// void PrecedenceBuilder::visit(StructElementAccess &node) {
//     rpn_.push_back(&node);
// }

void PrecedenceBuilder::visit(VariableAccess &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(StringLiteral &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::visit(IntegerLiteral &node) {
    rpn_.push_back(&node);
}

void PrecedenceBuilder::handle_operator(const OperatorData &operator_data) {
    if (!operator_data_.empty()) {
        while (
            operator_data_.top().precedence_ < operator_data.precedence_ ||
            (operator_data_.top().is_left_associativity_ &&
             operator_data_.top().precedence_ == operator_data.precedence_)) {
            rpn_.push_back(operator_data_.top().operator_node_);
            operator_data_.pop();
            if (operator_data_.empty()) {
                break;
            }
        }
    }
    operator_data_.push(operator_data);
}

} // namespace c::ast::detail