#pragma once

#include <memory>
#include <string>
#include <vector>

namespace c::ast {

class Visitor;

class Node {
  public:
    virtual ~Node() = default;
    virtual void accept(Visitor &visitor) = 0;
};

using Childs = std::vector<Node *>;

class Program final {
  public:
    template <class T, class... Args> T *create_node(Args &&...args) {
        static_assert(std::is_base_of_v<Node, T>);
        nodes_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return dynamic_cast<T *>(nodes_.back().get());
    }

    void set_childs(Childs childs) {
        childs_ = std::move(childs);
    }
    const Childs &get_childs() const {
        return childs_;
    }

  private:
    std::vector<std::unique_ptr<Node>> nodes_;
    Childs childs_;
};

class HeaderFile final : public Node {
  public:
    explicit HeaderFile(std::string file_name)
        : file_name_(std::move(file_name)) {}
    const std::string &file_name() const {
        return file_name_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string file_name_;
};

class FunctionDefinition final : public Node {
  public:
    FunctionDefinition(
        Node *return_type,
        std::string id,
        Childs actions,
        Childs args_declarations)
        : return_type_(return_type), id_(std::move(id)),
          actions_(std::move(actions)),
          args_declarations_(std::move(args_declarations)) {}
    const Childs &actions() const {
        return actions_;
    }
    const Childs &args_declarations() const {
        return args_declarations_;
    }
    Node *return_type() const {
        return return_type_;
    }
    const std::string &id() const {
        return id_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *return_type_;
    std::string id_;

    Childs actions_;
    Childs args_declarations_;
};

class LocalScope final : public Node {
  public:
    explicit LocalScope(Childs actions) : actions_(std::move(actions)) {}
    const Childs &actions() const {
        return actions_;
    }
    void accept(Visitor &visitor) override;

  private:
    Childs actions_;
};

// Expressions

class Expression final : public Node {
  public:
    explicit Expression(Node *expression) : expression_(expression) {}
    Node *expression() const {
        return expression_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *expression_;
};

class FunctionCall final : public Node {
  public:
    FunctionCall(std::string id, Childs args)
        : id_(std::move(id)), args_(std::move(args)) {}
    const std::string &id() const {
        return id_;
    }
    const Childs &args() const {
        return args_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string id_;
    Childs args_;
};

class VariableWriting final : public Node {
  public:
    explicit VariableWriting(Node *variable_writing)
        : variable_writing_(variable_writing) {}
    Node *variable_writing() const {
        return variable_writing_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *variable_writing_;
};

class DataCreate final : public Node {
  public:
    explicit DataCreate(Node *data_create) : data_create_(data_create) {}
    Node *data_create() const {
        return data_create_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *data_create_;
};

// Statements

class ReturnStatement final : public Node {
  public:
    explicit ReturnStatement(Node *value) : value_(value) {}
    Node *value() const {
        return value_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *value_;
};

class ForStatement final : public Node {
  public:
    ForStatement(
        Node *for_data_using, Node *truth_value, Node *value, Childs actions)
        : for_data_using_(for_data_using), truth_value_(truth_value),
          value_(value), actions_(std::move(actions)) {}
    Node *for_data_using() const {
        return for_data_using_;
    }
    Node *truth_value() const {
        return truth_value_;
    }
    Node *value() const {
        return value_;
    }
    const Childs &actions() const {
        return actions_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *for_data_using_;
    Node *truth_value_;
    Node *value_;

    Childs actions_;
};

class IfStatement final : public Node {
  public:
    IfStatement(Node *truth_value, Childs actions)
        : truth_value_(truth_value), actions_(std::move(actions)) {}
    Node *truth_value() const {
        return truth_value_;
    }
    const Childs &actions() const {
        return actions_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *truth_value_;

    Childs actions_;
};

class ContinueStatement final : public Node {
  public:
    const char *str() const {
        return "continue";
    }
    void accept(Visitor &visitor) override;
};

class BreakStatement final : public Node {
  public:
    const char *str() const {
        return "break";
    }
    void accept(Visitor &visitor) override;
};

// // Struct

// class StructDeclaration final : public Node {
//   public:
//     explicit StructDeclaration(std::string id) : id_(std::move(id)) {}
//     const std::string &id() const {
//         return id_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     std::string id_;
// };

// class StructDefinition final : public Node {
//   public:
//     StructDefinition(std::string id, std::string object, Childs data_uninit)
//         : id_(std::move(id)), object_(std::move(object)),
//           data_uninit_(std::move(data_uninit)) {}
//     const std::string &id() const {
//         return id_;
//     }
//     const std::string &object() const {
//         return object_;
//     }
//     const Childs &data_uninit() const {
//         return data_uninit_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     std::string id_;
//     std::string object_;
//     Childs data_uninit_;
// };

// class StructInit final : public Node {
//   public:
//     StructInit(Node *struct_type, std::string id, Childs values)
//         : struct_type_(struct_type), id_(std::move(id)),
//           values_(std::move(values)) {}
//     Node *struct_type() const {
//         return struct_type_;
//     }
//     const std::string &id() const {
//         return id_;
//     }
//     const Childs &values() const {
//         return values_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     Node *struct_type_;
//     std::string id_;
//     Childs values_;
// };

// class StructUninit final : public Node {
//   public:
//     StructUninit(Node *struct_type, std::string id)
//         : struct_type_(struct_type), id_(std::move(id)) {}
//     Node *struct_type() const {
//         return struct_type_;
//     }
//     const std::string &id() const {
//         return id_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     Node *struct_type_;
//     std::string id_;
// };

// class StructElementAccess final : public Node {
//   public:
//     explicit StructElementAccess(Childs lvalue_refer_stream)
//         : lvalue_refer_stream_(std::move(lvalue_refer_stream)) {}
//     const Childs &lvalue_refer_stream() const {
//         return lvalue_refer_stream_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     Childs lvalue_refer_stream_;
// };

// class StructType final : public Node {
//   public:
//     StructType(bool is_const, std::string id)
//         : is_const_(is_const), id_(std::move(id)) {}
//     bool is_const() const {
//         return is_const_;
//     }
//     const std::string &id() const {
//         return id_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     bool is_const_;
//     std::string id_;
// };

// class StructElementRefer final : public Node {
//   public:
//     explicit StructElementRefer(std::string element_refer)
//         : element_refer_(std::move(element_refer)) {}
//     const std::string &element_refer() const {
//         return element_refer_;
//     }
//     void accept(Visitor &visitor) override;

//   private:
//     std::string element_refer_;
// };

// Array

class ArrayUninit final : public Node {
  public:
    ArrayUninit(Node *type, std::string id, Node *size)
        : type_(type), id_(std::move(id)), size_(size) {}
    Node *type() const {
        return type_;
    }
    const std::string &id() const {
        return id_;
    }
    Node *size() const {
        return size_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *type_;
    std::string id_;
    Node *size_;
};

class ArrayElementAccess final : public Node {
  public:
    ArrayElementAccess(std::string id, Node *idx)
        : id_(std::move(id)), idx_(idx) {}
    const std::string &id() const {
        return id_;
    }
    Node *idx() const {
        return idx_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string id_;
    Node *idx_;
};

// Variable

class VariableInit final : public Node {
  public:
    VariableInit(Node *type, std::string id, Node *value)
        : type_(type), id_(std::move(id)), value_(value) {}
    Node *type() const {
        return type_;
    }
    const std::string &id() const {
        return id_;
    }
    Node *value() const {
        return value_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *type_;
    std::string id_;
    Node *value_;
};

class VariableUninit final : public Node {
  public:
    VariableUninit(Node *type, std::string id)
        : type_(type), id_(std::move(id)) {}
    Node *type() const {
        return type_;
    }
    const std::string &id() const {
        return id_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *type_;
    std::string id_;
};

class VariableAccess final : public Node {
  public:
    explicit VariableAccess(std::string id) : id_(std::move(id)) {}
    const std::string &id() const {
        return id_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string id_;
};

// Operations

class Assignment final : public Node {
  public:
    Assignment(Childs expression, Childs rpn)
        : expression_(std::move(expression)), rpn_(std::move(rpn)) {}
    const Childs &expression() const {
        return expression_;
    }
    const Childs &rpn() const {
        return rpn_;
    }
    void accept(Visitor &visitor) override;

  private:
    Childs expression_;
    // Reverse Polish notation
    Childs rpn_;
};

class RvalueOperation final : public Node {
  public:
    RvalueOperation(Childs expression, Childs rpn)
        : expression_(std::move(expression)), rpn_(std::move(rpn)) {}
    const Childs &expression() const {
        return expression_;
    }
    const Childs &rpn() const {
        return rpn_;
    }
    void accept(Visitor &visitor) override;

  private:
    Childs expression_;
    // Reverse Polish notation
    Childs rpn_;
};

class AssignmentOperator final : public Node {
  public:
    explicit AssignmentOperator(std::string assign_operator)
        : assign_operator_(std::move(assign_operator)) {}
    const std::string &assign_operator() const {
        return assign_operator_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string assign_operator_;
};

class ArithmeticOperator final : public Node {
  public:
    explicit ArithmeticOperator(std::string arithmetic_operator)
        : arithmetic_operator_(std::move(arithmetic_operator)) {}
    const std::string &arithmetic_operator() const {
        return arithmetic_operator_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string arithmetic_operator_;
};

class RelationalOperator final : public Node {
  public:
    explicit RelationalOperator(std::string relational_operator)
        : relational_operator_(std::move(relational_operator)) {}
    const std::string &relational_operator() const {
        return relational_operator_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string relational_operator_;
};

class LogicalOperator final : public Node {
  public:
    explicit LogicalOperator(std::string logical_operator)
        : logical_operator_(std::move(logical_operator)) {}
    const std::string &logical_operator() const {
        return logical_operator_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string logical_operator_;
};

class PrefixIncrement final : public Node {
  public:
    explicit PrefixIncrement(Node *value) : value_(value) {}
    Node *value() const {
        return value_;
    }
    const char *increment() const {
        return "++";
    }
    void accept(Visitor &visitor) override;

  private:
    Node *value_;
};

class PostfixIncrement final : public Node {
  public:
    explicit PostfixIncrement(Node *value) : value_(value) {}
    Node *value() const {
        return value_;
    }
    const char *increment() const {
        return "++";
    }
    void accept(Visitor &visitor) override;

  private:
    Node *value_;
};

class PrefixDecrement final : public Node {
  public:
    explicit PrefixDecrement(Node *value) : value_(value) {}
    Node *value() const {
        return value_;
    }
    const char *decrement() const {
        return "--";
    }
    void accept(Visitor &visitor) override;

  private:
    Node *value_;
};

class PostfixDecrement final : public Node {
  public:
    explicit PostfixDecrement(Node *value) : value_(value) {}
    Node *value() const {
        return value_;
    }
    const char *decrement() const {
        return "--";
    }
    void accept(Visitor &visitor) override;

  private:
    Node *value_;
};

// Types

class ArrayType final : public Node {
  public:
    ArrayType(bool is_const, Node *type) : is_const_(is_const), type_(type) {}
    bool is_const() const {
        return is_const_;
    }
    Node *type() const {
        return type_;
    }
    void accept(Visitor &visitor) override;

  private:
    bool is_const_;
    Node *type_;
};

class PointerType final : public Node {
  public:
    PointerType(bool is_const, Node *type, std::size_t level)
        : is_const_(is_const), type_(type), level_(level) {}
    bool is_const() const {
        return is_const_;
    }
    Node *type() const {
        return type_;
    }
    std::size_t level() const {
        return level_;
    }
    void accept(Visitor &visitor) override;

  private:
    bool is_const_;
    Node *type_;
    std::size_t level_;
};

class DataType final : public Node {
  public:
    DataType(bool is_const, Node *type) : is_const_(is_const), type_(type) {}
    bool is_const() const {
        return is_const_;
    }
    Node *type() const {
        return type_;
    }
    void accept(Visitor &visitor) override;

  private:
    bool is_const_;
    Node *type_;
};

class BaseType final : public Node {
  public:
    explicit BaseType(std::string type) : type_(std::move(type)) {}
    const std::string &type() const {
        return type_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string type_;
};

class VoidType final : public Node {
  public:
    const char *type() const {
        return "void";
    }
    void accept(Visitor &visitor) override;
};

// Literals

class StringLiteral final : public Node {
  public:
    explicit StringLiteral(std::string string) : string_(std::move(string)) {}
    const std::string &string() const {
        return string_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string string_;
};

class IntegerLiteral final : public Node {
  public:
    explicit IntegerLiteral(std::string integer)
        : integer_(std::move(integer)) {}
    const std::string &integer() const {
        return integer_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string integer_;
};

} // namespace c::ast