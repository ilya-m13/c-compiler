#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace c::ast::symtab {

class Type;
class Scope;

class Symbol {
  public:
    virtual ~Symbol() = default;

    void set_scope(Scope *scope) {
        scope_ = scope;
    }
    void set_insertion_order_num(std::size_t i) {
        insertion_order_ = i;
    }

    Scope *get_scope() const {
        return scope_;
    }
    std::size_t get_insertion_order_num() const {
        return insertion_order_;
    }

    virtual const std::string &get_name() const = 0;

  protected:
    Scope *scope_{nullptr};
    std::size_t insertion_order_{0};
};

class Scope {
  public:
    virtual ~Scope() = default;

    void set_enclosing_scope(Scope *enclosing_scope) {
        enclosing_scope_ = enclosing_scope;
    }
    void define(Symbol *sym) {
        symbols_[sym->get_name()] = sym;
        sym->set_scope(this);
    }
    void nest(std::unique_ptr<Scope> &&scope) {
        nested_scopes_.push_back(std::move(scope));
    }

    Scope *get_enclosing_scope() const {
        return enclosing_scope_;
    }
    const std::vector<std::unique_ptr<Scope>> &get_nested_scopes() const {
        return nested_scopes_;
    }
    std::vector<const Scope *> get_enclosing_path_to_root() const {
        std::vector<const Scope *> root_path;
        const auto *cur_scope = this;
        for (; cur_scope->get_enclosing_scope() != nullptr;
             cur_scope = cur_scope->get_enclosing_scope()) {
            root_path.push_back(cur_scope);
        }
        root_path.push_back(cur_scope);
        return root_path;
    }
    // Scope *get_outer_most_enclosing_scope() const;
    // Symbol *resolve(const std::string &name) const;
    // Symbol *get_symbol(const std::string &name) const;
    const std::unordered_map<std::string_view, Symbol *> &get_symbols() const {
        return symbols_;
    }
    std::size_t get_number_of_symbols() const {
        return symbols_.size();
    }

    virtual const std::string &get_name() const = 0;

  protected:
    Scope *enclosing_scope_{nullptr};
    std::unordered_map<std::string_view, Symbol *> symbols_;
    std::vector<std::unique_ptr<Scope>> nested_scopes_;
};

class LocalScope : public Scope {
  public:
    const std::string &get_name() const override {
        return name_;
    }

  private:
    std::string name_{"local"};
};

class TypedSymbol {
  public:
    virtual ~TypedSymbol() = default;

    virtual void set_type(std::unique_ptr<Type> &&type) = 0;

    virtual Type *get_type() const = 0;
};

class SymbolWithScope : public Scope, public Symbol {};

class FunctionSymbol : public SymbolWithScope, public TypedSymbol {
  public:
    explicit FunctionSymbol(std::string name) : name_(std::move(name)) {
        set_scope(this);
    }

    void set_number_of_param(std::size_t param_num) {
        param_num_ = param_num;
    }

    std::size_t get_number_of_var() const {
        return symbols_.size() - param_num_;
    }
    std::size_t get_number_of_param() const {
        return param_num_;
    }

    void set_type(std::unique_ptr<Type> &&type) override {
        type_ = std::move(type);
    }

    Type *get_type() const override {
        return type_.get();
    }
    const std::string &get_name() const override {
        return name_;
    }

  protected:
    std::string name_;
    std::unique_ptr<Type> type_{nullptr};
    std::size_t param_num_{0};
};

class VariableSymbol : public Symbol, public TypedSymbol {
  public:
    explicit VariableSymbol(std::string name) : name_(std::move(name)) {}

    void set_type(std::unique_ptr<Type> &&type) override {
        type_ = std::move(type);
    }

    Type *get_type() const override {
        return type_.get();
    }
    const std::string &get_name() const override {
        return name_;
    }

  protected:
    std::string name_;
    std::unique_ptr<Type> type_{nullptr};
};

// class ParameterSymbol : public VariableSymbol {
//   public:
//     explicit ParameterSymbol(std::string name)
//         : VariableSymbol(std::move(name)) {}
// };

// class FieldSymbol : public VariableSymbol {};

// class StructSymbol {};

class Type {
  public:
    virtual ~Type() = default;

    virtual const std::string &get_name() const = 0;
    virtual const char *get_type() const = 0;
    virtual bool is_const() const = 0;
};

class ArrayType : public Type {
  public:
    ArrayType(std::string name, bool is_const)
        : name_(std::move(name)), is_const_(is_const) {}

    const std::string &get_name() const override {
        return name_;
    }
    const char *get_type() const override {
        return "[]";
    }
    bool is_const() const override {
        return is_const_;
    }

  private:
    std::string name_;
    bool is_const_;
};

class PointerType : public Type {
  public:
    PointerType(std::string name, std::size_t level, bool is_const)
        : name_(std::move(name)), level_(level), is_const_(is_const) {}

    std::size_t get_level() const {
        return level_;
    }

    const std::string &get_name() const override {
        return name_;
    }
    const char *get_type() const override {
        return "*";
    }
    bool is_const() const override {
        return is_const_;
    }

  private:
    std::string name_;
    std::size_t level_;
    bool is_const_;
};

class PrimitiveType : public Type {
  public:
    explicit PrimitiveType(std::string name, bool is_const)
        : name_(std::move(name)), is_const_(is_const) {}

    const std::string &get_name() const override {
        return name_;
    }
    const char *get_type() const override {
        return "var";
    }
    bool is_const() const override {
        return is_const_;
    }

  private:
    std::string name_;
    bool is_const_;
};

} // namespace c::ast::symtab