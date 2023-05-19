#include <libc/ast/ast.hpp>
#include <libc/ast/symtab/symtab.hpp>

namespace c {

void generate(
    std::ostream &ir, ast::Program &program, ast::symtab::Symtab &symtab);

} // namespace c