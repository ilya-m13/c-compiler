#include <libc/ast/code_generator.hpp>

#include <cctype>
#include <iomanip>

namespace c::ast {

const std::unordered_map<std::string, std::string> c_ir_types = {
    {"int", "i32"},
    {"char", "i8"},
    {"float", "float"},
    {"double", "double"},
    {"short", "i16"},
    {"long", "i64"},
    {"void", "void"}};

const std::unordered_map<std::string, std::string> c_ir_arth_i = {
    {"+", "add"}, {"-", "sub"}, {"*", "mul"}, {"/", "sdiv"}, {"%", "srem"}};

const std::unordered_map<std::string, std::string> c_ir_arth_f = {
    {"+", "fadd"}, {"-", "fsub"}, {"*", "fmul"}, {"/", "fdiv"}};

const std::unordered_map<std::string, std::string> c_ir_rel_i = {
    {"==", "icmp eq"},
    {"!=", "icmp ne"},
    {"<", "icmp slt"},
    {"<=", "icmp sle"},
    {">", "icmp sgt"},
    {">=", "icmp sge"}};

const std::unordered_map<std::string, std::string> c_ir_rel_f = {
    {"==", "fcmp oeq"},
    {"!=", "fcmp une"},
    {"<", "fcmp olt"},
    {"<=", "fcmp ole"},
    {">", "fcmp ogt"},
    {">=", "fcmp oge"}};

const std::unordered_map<std::string, std::size_t> c_ir_type_order = {
    {"i1", 0},
    {"i8", 1},
    {"i16", 2},
    {"i32", 3},
    {"i64", 4},
    {"float", 5},
    {"double", 6}};

void CodeGenerator::exec(
    std::ostream &os, Program &program, symtab::Symtab &symtab) {
    DeclareStr::exec(os, program);
    CodeGenerator code_generator(os, symtab);
    for (auto *child : program.get_childs()) {
        child->accept(code_generator);
    }
}

void CodeGenerator::visit(FunctionDefinition &node) {
    auto *func_sym = get_funcsym(node.id());
    scopes_.push(func_sym);

    cgs_alc_[func_sym].name_ = "@" + func_sym->get_name();
    cgs_alc_[func_sym].type_ = get_ir_type(func_sym->get_type());
    ir_ << "define " << cgs_alc_[func_sym].type_ << " "
        << cgs_alc_[func_sym].name_ << "(";

    auto params = func_sym->get_params();
    {
        symtab::VariableSymbol *param = nullptr;
        switch (params.size()) {
        case 0:
            break;
        case 1:
            param = get_varsym(params[0]->get_name());
            cgs_alc_[param].name_ = "%" + param->get_name();
            cgs_alc_[param].type_ = get_ir_type(param->get_type());
            ir_ << cgs_alc_[param].type_ << " " << cgs_alc_[param].name_;
            break;
        default:
            param = get_varsym(params[0]->get_name());
            cgs_alc_[param].name_ = "%" + param->get_name();
            cgs_alc_[param].type_ = get_ir_type(param->get_type());
            ir_ << cgs_alc_[param].type_ << " " << cgs_alc_[param].name_;
            for (std::size_t i = 1; i < params.size(); ++i) {
                param = get_varsym(params[i]->get_name());
                cgs_alc_[param].name_ = "%" + param->get_name();
                cgs_alc_[param].type_ = get_ir_type(param->get_type());
                ir_ << ", " << cgs_alc_[param].type_ << " "
                    << cgs_alc_[param].name_;
            }
        }
    }
    ir_ << ") {\nentry:\n";

    for (auto *param : params) {
        std::string alloca_name =
            cgs_alc_[param].name_ + ".addr" + std::to_string(addr_num_++);
        ir_ << "\t" << alloca_name << " = alloca " << cgs_alc_[param].type_
            << "\n";
        ir_ << "\t"
            << "store " << cgs_alc_[param].type_ << " " << cgs_alc_[param].name_
            << ", ";
        cgs_alc_[param].name_ = std::move(alloca_name);
        ir_ << cgs_alc_[param].type_ + "* " << cgs_alc_[param].name_ << "\n";
    }
    ir_ << "\n";

    for (auto *action : node.actions()) {
        action->accept(*this);
    }

    ir_ << "}\n\n";
    scopes_.pop();
    scope_order_ = 0;
}

void CodeGenerator::visit(LocalScope &node) {
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

void CodeGenerator::visit(Expression &node) {
    node.expression()->accept(*this);
    ir_ << "\n";
}

void CodeGenerator::visit(FunctionCall &node) {
    bool prev_rvalue_oper = is_rvalue_oper_;
    std::vector<IrNode> ir_args;
    for (auto *arg : node.args()) {
        is_rvalue_oper_ = false;
        arg->accept(*this);
        ir_args.push_back(std::move(ir_buf_));
    }
    is_rvalue_oper_ = prev_rvalue_oper;

    if (node.id() == "printf") {
        IrNode ir_var("%tmp" + std::to_string(tmp_num_++), "i32");
        ir_ << "\t" << ir_var.name_
            << " = call i32 (i8*, ...) @printf(i8* getelementptr ("
            << ir_args[0].type_ << ", " << ir_args[0].type_ + "* "
            << ir_args[0].name_ << ", i64 0, i64 0)";
        for (std::size_t i = 1; i < ir_args.size(); ++i) {
            ir_ << ", " << ir_args[i].type_ << " " << ir_args[i].name_;
        }
        ir_ << ")\n";
        if (is_rvalue_oper_) {
            calc_expr_.push(std::move(ir_var));
            return;
        }
        ir_buf_ = std::move(ir_var);
        return;
    }

    auto *func = get_funcsym(node.id());

    auto params = func->get_params();
    for (std::size_t i = 0; i < params.size(); ++i) {
        auto *param = dynamic_cast<symtab::VariableSymbol *>(params[i]);
        auto param_type = get_ir_type(param->get_type());
        if (param_type != ir_args[i].type_) {
            cast_to(IrNode("", param_type), ir_args[i]);
        }
    }

    IrNode ir_var;
    if (cgs_alc_[func].type_ != "void") {
        ir_var.name_ = "%tmp" + std::to_string(tmp_num_++);
        ir_var.type_ = cgs_alc_[func].type_;

        ir_ << "\t" << ir_var.name_ << " = call " << ir_var.type_ << " "
            << cgs_alc_[func].name_ << "(";
    } else {
        ir_ << "\t"
            << "call " << cgs_alc_[func].type_ << " " << cgs_alc_[func].name_
            << "(";
    }
    switch (ir_args.size()) {
    case 0:
        ir_ << ")\n";
        break;
    case 1:
        ir_ << ir_args[0].type_ << " " << ir_args[0].name_ << ")\n";
        break;
    default:
        ir_ << ir_args[0].type_ << " " << ir_args[0].name_;
        for (std::size_t i = 1; i < ir_args.size(); ++i) {
            ir_ << ", " << ir_args[i].type_ << " " << ir_args[i].name_;
        }
        ir_ << ")\n";
    }

    if (is_rvalue_oper_) {
        calc_expr_.push(std::move(ir_var));
        return;
    }
    ir_buf_ = std::move(ir_var);
}

void CodeGenerator::visit(VariableWriting &node) {
    node.variable_writing()->accept(*this);
}

void CodeGenerator::visit(DataCreate &node) {
    node.data_create()->accept(*this);
}

// Statements

void CodeGenerator::visit(ReturnStatement &node) {
    node.value()->accept(*this);
    ir_ << "\t"
        << "ret " << ir_buf_.type_ << " " << ir_buf_.name_ << "\n";
}

void CodeGenerator::visit(ForStatement &node) {
    scopes_.push(scopes_.top()->get_nested_scopes()[scope_order_++].get());
    std::size_t prev_scope_order = scope_order_;
    scope_order_ = 0;

    std::string cmp = "block" + std::to_string(block_num_++);
    std::string scope = "block" + std::to_string(block_num_++);
    std::string skip = "block" + std::to_string(block_num_);
    skip_nums_.push_back(block_num_++);
    std::string loop = "block" + std::to_string(block_num_);
    loop_nums_.push_back(block_num_++);

    if (node.for_data_using() != nullptr) {
        node.for_data_using()->accept(*this);
    }
    ir_ << "\t"
        << "br label %" << cmp << "\n\n";

    ir_ << cmp << ":\n";
    if (node.truth_value() != nullptr) {
        is_rel_op_last_ = false;
        node.truth_value()->accept(*this);
        if (!is_rel_op_last_) {
            std::string tmp_name = "%tmp" + std::to_string(tmp_num_++);
            std::string cmp;
            std::string zero;
            if (ir_buf_.type_[0] == 'i') {
                cmp = "icmp ne";
                zero = "0";
            } else {
                cmp = "fcmp une";
                zero = "0.0";
            }

            ir_ << "\t" << tmp_name << " = " << cmp << " " << ir_buf_.type_
                << " " << ir_buf_.name_ << ", " << zero << "\n";

            ir_buf_.name_ = std::move(tmp_name);
            ir_buf_.type_ = "i1";
        }
        ir_ << "\t"
            << "br i1 " << ir_buf_.name_ << ", label %" << scope << ", label %"
            << skip << "\n\n";
    } else {
        ir_ << "\t"
            << "br label %" << scope << "\n\n";
    }

    ir_ << scope << ":\n";
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    loop_nums_.pop_back();
    skip_nums_.pop_back();
    ir_ << "\t"
        << "br label %" << loop << "\n\n";

    ir_ << loop << ":\n";
    if (node.value() != nullptr) {
        node.value()->accept(*this);
    }
    ir_ << "\t"
        << "br label %" << cmp << "\n\n";

    ir_ << skip << ":\n";

    scope_order_ = prev_scope_order;
    scopes_.pop();
}

void CodeGenerator::visit(IfStatement &node) {
    scopes_.push(scopes_.top()->get_nested_scopes()[scope_order_++].get());
    std::size_t prev_scope_order = scope_order_;
    scope_order_ = 0;

    is_rel_op_last_ = false;
    node.truth_value()->accept(*this);
    if (!is_rel_op_last_) {
        std::string tmp_name = "%tmp" + std::to_string(tmp_num_++);
        std::string cmp;
        std::string zero;
        if (ir_buf_.type_ == std::string("double") ||
            ir_buf_.type_ == std::string("float")) {
            cmp = "fcmp une";
            zero = "0.0";
        } else {
            cmp = "icmp ne";
            zero = "0";
        }

        ir_ << "\t" << tmp_name << " = " << cmp << " " << ir_buf_.type_ << " "
            << ir_buf_.name_ << ", " << zero << "\n";

        ir_buf_.name_ = std::move(tmp_name);
        ir_buf_.type_ = "i1";
    }

    std::string scope = "block" + std::to_string(block_num_++);
    std::string skip = "block" + std::to_string(block_num_++);
    ir_ << "\t"
        << "br i1 " << ir_buf_.name_ << ", label %" << scope << ", label %"
        << skip << "\n\n";

    ir_ << scope << ":\n";
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
    ir_ << "\t"
        << "br label %" << skip << "\n\n";

    ir_ << skip << ":\n";

    scope_order_ = prev_scope_order;
    scopes_.pop();
}

void CodeGenerator::visit(ContinueStatement & /*node*/) {
    ir_ << "\t"
        << "br label %block" << std::to_string(loop_nums_.back()) << "\n";
}

void CodeGenerator::visit(BreakStatement & /*node*/) {
    ir_ << "\t"
        << "br label %block" << std::to_string(skip_nums_.back()) << "\n";
}

// Array

void CodeGenerator::visit(ArrayUninit &node) {
    auto *var = get_varsym(node.id());
    cgs_alc_[var].name_ =
        "%" + var->get_name() + ".addr" + std::to_string(addr_num_++);
    cgs_alc_[var].type_ = get_ir_type(var->get_type());

    node.size()->accept(*this);
    if (std::string("i64") != ir_buf_.type_) {
        cast_to(IrNode("", "i64"), ir_buf_);
    }

    ir_ << "\t" << cgs_alc_[var].name_ << " = alloca " << cgs_alc_[var].type_
        << ", i64 " << ir_buf_.name_ << "\n";
}

void CodeGenerator::visit(ArrayElementAccess &node) {
    bool prev_rvalue_oper = is_rvalue_oper_;
    is_rvalue_oper_ = false;
    node.idx()->accept(*this);
    is_rvalue_oper_ = prev_rvalue_oper;

    if (std::string("i64") != ir_buf_.type_) {
        cast_to(IrNode("", "i64"), ir_buf_);
    }

    auto *var = get_varsym(node.id());

    std::string tmp_name;
    IrNode ir_var;
    if (cgs_alc_[var].type_.back() == '*') {
        std::string prev_tmp_name = "%tmp" + std::to_string(tmp_num_++);
        ir_ << "\t" << prev_tmp_name << " = load " << cgs_alc_[var].type_
            << ", " << cgs_alc_[var].type_ + "* " << cgs_alc_[var].name_
            << "\n";
        tmp_name = "%tmp" + std::to_string(tmp_num_++);
        ir_ << "\t" << tmp_name << " = getelementptr "
            << cgs_alc_[var].type_.substr(0, cgs_alc_[var].type_.size() - 1)
            << ", " << cgs_alc_[var].type_ << " " << prev_tmp_name << ", i64 "
            << ir_buf_.name_ << "\n";
        ir_var.type_ =
            cgs_alc_[var].type_.substr(0, cgs_alc_[var].type_.size() - 1);
    } else {
        tmp_name = "%tmp" + std::to_string(tmp_num_++);
        ir_ << "\t" << tmp_name << " = getelementptr " << cgs_alc_[var].type_
            << ", " << cgs_alc_[var].type_ + "* " << cgs_alc_[var].name_
            << ", i64 " << ir_buf_.name_ << "\n";
        ir_var.type_ = cgs_alc_[var].type_;
    }

    ir_var.name_ = "%tmp" + std::to_string(tmp_num_++);
    ir_var.alc_name_ = std::move(tmp_name);
    ir_ << "\t" << ir_var.name_ << " = load " << ir_var.type_ << ", "
        << ir_var.type_ + "* " << ir_var.alc_name_ << "\n";

    if (is_rvalue_oper_) {
        calc_expr_.emplace(std::move(ir_var));
        return;
    }
    ir_buf_ = std::move(ir_var);
}

// Variable

void CodeGenerator::visit(VariableInit &node) {
    auto *var = get_varsym(node.id());
    cgs_alc_[var].name_ =
        "%" + var->get_name() + ".addr" + std::to_string(addr_num_++);
    cgs_alc_[var].type_ = get_ir_type(var->get_type());

    ir_ << "\t" << cgs_alc_[var].name_ << " = alloca " << cgs_alc_[var].type_
        << "\n";

    node.value()->accept(*this);
    if (cgs_alc_[var].type_ != ir_buf_.type_) {
        cast_to(cgs_alc_[var], ir_buf_);
    }
    ir_ << "\t"
        << "store " << cgs_alc_[var].type_ << " " << ir_buf_.name_ << ", "
        << cgs_alc_[var].type_ + "* " << cgs_alc_[var].name_ << "\n";
}

void CodeGenerator::visit(VariableUninit &node) {
    auto *var = get_varsym(node.id());
    cgs_alc_[var].name_ =
        "%" + var->get_name() + ".addr" + std::to_string(addr_num_++);
    cgs_alc_[var].type_ = get_ir_type(var->get_type());

    ir_ << "\t" << cgs_alc_[var].name_ << " = alloca " << cgs_alc_[var].type_
        << "\n";
}

void CodeGenerator::visit(VariableAccess &node) {
    auto *var = get_varsym(node.id());
    IrNode ir_var(
        "%tmp" + std::to_string(tmp_num_++),
        cgs_alc_[var].type_,
        cgs_alc_[var].name_);
    ir_ << "\t" << ir_var.name_ << " = load " << ir_var.type_ << ", "
        << ir_var.type_ + "* " << ir_var.alc_name_ << "\n";
    if (is_rvalue_oper_) {
        calc_expr_.emplace(std::move(ir_var));
        return;
    }
    ir_buf_ = std::move(ir_var);
}

// Operations

void CodeGenerator::visit(Assignment &node) {
    is_rvalue_oper_ = true;
    for (auto *value : node.rpn()) {
        value->accept(*this);
        is_rvalue_oper_ = true;
        if (dynamic_cast<RvalueOperation *>(value) != nullptr) {
            calc_expr_.push(std::move(ir_buf_));
        }
    }
    is_rvalue_oper_ = false;

    ir_buf_ = std::move(calc_expr_.top());
    calc_expr_.pop();

    if (dynamic_cast<RelationalOperator *>(node.rpn().back()) != nullptr) {
        is_rel_op_last_ = true;
    }
}

void CodeGenerator::visit(RvalueOperation &node) {
    for (auto *value : node.rpn()) {
        is_rvalue_oper_ = true;
        value->accept(*this);
    }
    is_rvalue_oper_ = false;

    ir_buf_ = std::move(calc_expr_.top());
    calc_expr_.pop();

    if (dynamic_cast<RelationalOperator *>(node.rpn().back()) != nullptr) {
        is_rel_op_last_ = true;
    }
}

void CodeGenerator::visit(AssignmentOperator &node) {
    IrNode rhs(std::move(calc_expr_.top()));
    calc_expr_.pop();
    IrNode lhs(std::move(calc_expr_.top()));
    calc_expr_.pop();

    if (lhs.type_ != rhs.type_) {
        cast_to(lhs, rhs);
    }

    if (node.assign_operator()[0] != '=') {
        calc_expr_.push(lhs);
        calc_expr_.push(rhs);

        ArithmeticOperator arth_op(node.assign_operator().substr(0, 1));
        arth_op.accept(*this);

        rhs = std::move(calc_expr_.top());
        calc_expr_.pop();
    }
    ir_ << "\t"
        << "store " << lhs.type_ << " " << rhs.name_ << ", " << lhs.type_ + "* "
        << lhs.alc_name_ << "\n";

    calc_expr_.push(std::move(rhs));
}

void CodeGenerator::visit(ArithmeticOperator &node) {
    IrNode rhs(std::move(calc_expr_.top()));
    calc_expr_.pop();
    IrNode lhs(std::move(calc_expr_.top()));
    calc_expr_.pop();

    if (lhs.type_ != rhs.type_) {
        cast(lhs, rhs);
    }

    IrNode res("%tmp" + std::to_string(tmp_num_++), lhs.type_);
    auto inst = lhs.type_[0] == 'i'
        ? c_ir_arth_i.at(node.arithmetic_operator())
        : c_ir_arth_f.at(node.arithmetic_operator());
    ir_ << "\t" << res.name_ << " = " + inst + " " << res.type_ << " "
        << lhs.name_ << ", " << rhs.name_ << "\n";

    calc_expr_.push(std::move(res));
}

void CodeGenerator::visit(RelationalOperator &node) {
    IrNode rhs(std::move(calc_expr_.top()));
    calc_expr_.pop();
    IrNode lhs(std::move(calc_expr_.top()));
    calc_expr_.pop();

    if (lhs.type_ != rhs.type_) {
        cast(lhs, rhs);
    }

    IrNode res("%tmp" + std::to_string(tmp_num_++), "i1");
    auto inst = lhs.type_[0] == 'i' ? c_ir_rel_i.at(node.relational_operator())
                                    : c_ir_rel_f.at(node.relational_operator());
    ir_ << "\t" << res.name_ << " = " + inst + " " << lhs.type_ << " "
        << lhs.name_ << ", " << rhs.name_ << "\n";

    calc_expr_.push(std::move(res));
}

// Literals

void CodeGenerator::visit(StringLiteral &node) {
    auto str = node.string();
    std::size_t n = 0;
    for (auto pos = str.find("\\n"); pos != std::string::npos;
         pos = str.find("\\n")) {
        str.replace(pos, 2, "\n");
        ++n;
    }
    ir_buf_.name_ = "@.str" + std::to_string(str_num_++);
    ir_buf_.type_ =
        "[" + std::to_string(node.string().size() - n + 1) + " x i8]";
}

void CodeGenerator::visit(IntegerLiteral &node) {
    if (is_rvalue_oper_) {
        calc_expr_.emplace(node.integer(), "i32");
        return;
    }
    ir_buf_.name_ = node.integer();
    ir_buf_.type_ = "i32";
    ir_buf_.alc_name_ = "";
}

// private methods

symtab::VariableSymbol *CodeGenerator::get_varsym(const std::string &id) {
    return dynamic_cast<symtab::VariableSymbol *>(scopes_.top()->resolve(id));
}

symtab::FunctionSymbol *CodeGenerator::get_funcsym(const std::string &id) {
    symtab::FunctionSymbol *func_sym = nullptr;
    for (auto *stack_node = symtab_.find_sym(id); stack_node != nullptr;
         stack_node = stack_node->prev_) {
        if (func_sym =
                dynamic_cast<symtab::FunctionSymbol *>(stack_node->sym_.get());
            func_sym != nullptr) {
            return func_sym;
        }
    }
    return func_sym;
}

std::string CodeGenerator::get_ir_type(symtab::Type *type) {
    std::string type_name = c_ir_types.at(type->get_name());
    if (type->get_type() == std::string("*")) {
        auto *pointer_type = dynamic_cast<symtab::PointerType *>(type);
        for (std::size_t i = 0; i < pointer_type->get_level(); ++i) {
            type_name.push_back('*');
        }
    }
    return type_name;
}

void CodeGenerator::cast(IrNode &lhs, IrNode &rhs) {
    std::string ir_name = "%tmp" + std::to_string(tmp_num_++);
    if (lhs.type_[0] == rhs.type_[0]) {
        if (c_ir_type_order.at(lhs.type_) < c_ir_type_order.at(rhs.type_)) {
            cast_out(lhs, rhs, ir_name, "sext");
        } else {
            cast_out(rhs, lhs, ir_name, "sext");
        }
    } else if (
        (lhs.type_[0] == 'd' && rhs.type_[0] == 'f') ||
        (lhs.type_[0] == 'f' && rhs.type_[0] == 'd')) {
        if (c_ir_type_order.at(lhs.type_) < c_ir_type_order.at(rhs.type_)) {
            cast_out(lhs, rhs, ir_name, "fpext");
        } else {
            cast_out(rhs, lhs, ir_name, "fpext");
        }
    } else {
        if (lhs.type_[0] == 'i') {
            cast_out(lhs, rhs, ir_name, "sitofp");
        } else {
            cast_out(rhs, lhs, ir_name, "sitofp");
        }
    }
}

void CodeGenerator::cast_to(const IrNode &to, IrNode &from) {
    std::string ir_name = "%tmp" + std::to_string(tmp_num_++);
    if (to.type_[0] == from.type_[0]) {
        if (c_ir_type_order.at(to.type_) < c_ir_type_order.at(from.type_)) {
            cast_out(from, to, ir_name, "trunc");
        } else {
            cast_out(from, to, ir_name, "sext");
        }
    } else if (
        (to.type_[0] == 'd' && from.type_[0] == 'f') ||
        (to.type_[0] == 'f' && from.type_[0] == 'd')) {
        if (c_ir_type_order.at(to.type_) < c_ir_type_order.at(from.type_)) {
            cast_out(from, to, ir_name, "fptrunc");
        } else {
            cast_out(from, to, ir_name, "fpext");
        }
    } else {
        if (to.type_[0] == 'i') {
            cast_out(from, to, ir_name, "fptosi");
        } else {
            cast_out(from, to, ir_name, "sitofp");
        }
    }
}

void CodeGenerator::cast_out(
    IrNode &from, const IrNode &to, std::string &ir_name, std::string inst) {
    if (std::isdigit(from.name_[0]) != 0) {
        if (from.type_[0] != to.type_[0]) {
            from.name_ += ".0";
        }
        from.type_ = to.type_;
        return;
    }
    if (from.type_ == std::string("i1")) {
        if (inst == std::string("sext")) {
            inst = "zext";
        }
    }
    ir_ << "\t" << ir_name << " = " << inst << " " << from.type_ << " "
        << from.name_ << " to " << to.type_ << "\n";
    from.name_ = std::move(ir_name);
    from.type_ = to.type_;
}

// DeclareStr

void DeclareStr::exec(std::ostream &os, Program &program) {
    DeclareStr declare_str(os);
    declare_str.ir_ << "target triple = \"x86_64-pc-linux-gnu\"\n\n";
    for (auto *child : program.get_childs()) {
        child->accept(declare_str);
    }
    declare_str.ir_ << "\n";
}

void DeclareStr::visit(FunctionDefinition &node) {
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
}

void DeclareStr::visit(LocalScope &node) {
    for (auto *action : node.actions()) {
        action->accept(*this);
    }
}

void DeclareStr::visit(Expression &node) {
    node.expression()->accept(*this);
}

void DeclareStr::visit(FunctionCall &node) {
    if (!is_printf_exist_ && node.id() == "printf") {
        is_printf_exist_ = true;
        ir_ << "declare i32 @printf(i8*, ...)\n\n";
    }
    for (auto *arg : node.args()) {
        arg->accept(*this);
    }
}

void DeclareStr::visit(VariableWriting &node) {
    node.variable_writing()->accept(*this);
}

void DeclareStr::visit(DataCreate &node) {
    node.data_create()->accept(*this);
}

void DeclareStr::visit(ForStatement &node) {
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
}

void DeclareStr::visit(IfStatement &node) {
    node.truth_value()->accept(*this);

    for (auto *action : node.actions()) {
        action->accept(*this);
    }
}

void DeclareStr::visit(VariableInit &node) {
    node.value()->accept(*this);
}

void DeclareStr::visit(StringLiteral &node) {
    auto str = node.string();
    std::size_t n = 0;
    for (auto pos = str.find("\\n"); pos != std::string::npos;
         pos = str.find("\\n")) {
        str.replace(pos, 2, "\n");
        ++n;
    }
    ir_ << "@.str" + std::to_string(str_num_++)
        << " = private unnamed_addr constant [" << node.string().size() - n + 1
        << " x i8] c\"";
    for (char c : str) {
        if (c == '\n') {
            ir_ << "\\0A";
        } else {
            ir_ << c;
        }
    }
    ir_ << "\\00\"\n";
}

} // namespace c::ast