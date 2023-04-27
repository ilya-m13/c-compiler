#pragma once

#include <libc/ast/symtab/symbols.hpp>

#include <array>
#include <cstdint>
#include <stack>
#include <stdexcept>
#include <string>

namespace c::ast::symtab {

const std::size_t c_table_size = 256;

class UndefinedReference : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};
class SymbolRedefinition : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};
class Overflow : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

struct StackNode {
    StackNode(std::unique_ptr<Symbol> &&sym, StackNode *prev, StackNode *bottom)
        : sym_(std::move(sym)), prev_(prev), bottom_(bottom) {}

    std::unique_ptr<Symbol> sym_;
    StackNode *prev_;
    StackNode *bottom_;
};

class Symtab {
  public:
    void add_sym(std::unique_ptr<Symbol> &&symbol);
    StackNode *find_sym(const std::string &name);

    const StackNode *top() const {
        return &symbols_.top();
    }

  private:
    std::uint8_t hash_code(const std::string &name);

    std::array<StackNode *, c_table_size> table_{nullptr};
    std::stack<StackNode> symbols_;
};

} // namespace c::ast::symtab