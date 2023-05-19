#include <libc/code_generator.hpp>

#include <libc/ast/code_generator.hpp>

namespace c {

void generate(
    std::ostream &ir, ast::Program &program, ast::symtab::Symtab &symtab) {
    c::ast::CodeGenerator::exec(ir, program, symtab);
}

} // namespace c