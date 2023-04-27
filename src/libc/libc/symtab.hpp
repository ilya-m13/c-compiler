#pragma once

#include <libc/ast/ast.hpp>
#include <libc/ast/symtab/symtab.hpp>

#include <ostream>

namespace c {

ast::symtab::Symtab get_symtab(ast::Program &program);

void dump_symtab(const ast::symtab::Symtab &symtab, std::ostream &os);

} // namespace c