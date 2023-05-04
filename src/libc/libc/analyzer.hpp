#pragma once

#include <libc/ast/type_analyzer.hpp>

namespace c {

void analyze(ast::Program &program, ast::symtab::Symtab &symtab);

} // namespace c