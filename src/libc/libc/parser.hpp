#pragma once

#include <libc/ast/ast.hpp>

#include <iosfwd>

namespace c {

struct Error {
    size_t line_;
    size_t column_;
    std::string message_;
};

using Errors = std::vector<Error>;

struct ParseResult {
    static ParseResult program(ast::Program program) {
        ParseResult result;
        result.program_ = std::move(program);
        return result;
    }

    static ParseResult errors(Errors errors) {
        ParseResult result;
        result.errors_ = std::move(errors);
        return result;
    }

    ast::Program program_;
    Errors errors_;
};

ParseResult parse(std::istream &in);

void dump_ast(ast::Program &program, std::ostream &out);
void dump_errors(const Errors &errors, std::ostream &out);

} // namespace c