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
    FunctionDefinition(std::string return_type, std::string id, Childs actions)
        : return_type_(std::move(return_type)), id_(std::move(id)),
          actions_(std::move(actions)) {}
    const Childs &actions() const {
        return actions_;
    }
    const std::string &return_type() const {
        return return_type_;
    }
    const std::string &id() const {
        return id_;
    }
    void accept(Visitor &visitor) override;

  private:
    std::string return_type_;
    std::string id_;

    Childs actions_;
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

class Arg final : public Node {
  public:
    explicit Arg(Node *arg) : arg_(arg) {}
    Node *arg() const {
        return arg_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *arg_;
};

class ReturnStatement final : public Node {
  public:
    explicit ReturnStatement(Node *literal) : literal_(literal) {}
    Node *literal() const {
        return literal_;
    }
    void accept(Visitor &visitor) override;

  private:
    Node *literal_;
};

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