#include <libc/symtab.hpp>

#include <libc/ast/symtab/detail/builder.hpp>

namespace c {

static std::string convert_root_path_to_string(
    const std::vector<const ast::symtab::Scope *> &root_path) {
    std::string root_path_str = (*root_path.rbegin())->get_name();
    for (auto it = ++root_path.rbegin(); it != root_path.rend(); ++it) {
        root_path_str += "::" + (*it)->get_name();
    }
    return root_path_str;
}

ast::symtab::Symtab get_symtab(ast::Program &program) {
    ast::symtab::Symtab symtab;
    ast::symtab::detail::Builder builder(symtab);
    builder.visit(program);
    return symtab;
}

void dump_symtab(const ast::symtab::Symtab &symtab, std::ostream &os) {
    const auto *top = symtab.top();
    for (; top != nullptr; top = top->bottom_) {
        auto *sym = top->sym_.get();
        os << "name=" << sym->get_name() << "  ";
        os << "in_order=" << sym->get_insertion_order_num() << "  ";
        if (auto *var = dynamic_cast<ast::symtab::VariableSymbol *>(sym);
            var != nullptr) {
            auto *type = var->get_type();
            os << "type-name=" << type->get_name() << "  ";
            os << "type=" << type->get_type() << "  ";
            os << "const=" << type->is_const() << "  ";
        } else if (auto *func =
                       dynamic_cast<ast::symtab::FunctionSymbol *>(sym);
                   func != nullptr) {
            auto *type = func->get_type();
            os << "type-name=" << type->get_name() << "  ";
            os << "type=" << type->get_type() << "  ";
            os << "const=" << type->is_const() << "  ";
            os << "param-num=" << func->get_number_of_param() << "  ";
            os << "sym-num=" << func->get_number_of_symbols() << "  ";
        }
        os << "root-path="
           << convert_root_path_to_string(
                  sym->get_scope()->get_enclosing_path_to_root())
           << "  ";
        os << "syn-num-in-scope=" << sym->get_scope()->get_number_of_symbols()
           << "\n";
    }
}

} // namespace c