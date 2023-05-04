#include <libc/analyzer.hpp>

namespace c {

void analyze(ast::Program &program, ast::symtab::Symtab &symtab) {
    ast::TypeAnalyzer::exec(program, symtab);
}

} // namespace c