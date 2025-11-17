#pragma once
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include "Lexer.hpp"

class TypeASTNode;
class ArrayASTNode;

struct Symbol {
    std::string name;
    std :: shared_ptr<TypeASTNode> type;
    std :: shared_ptr<ArrayASTNode> m_array;
    bool isVarOrArray;
    bool isLoaded;
    llvm::AllocaInst* store;
};

struct GenContext {
    GenContext(const std::string& moduleName);

    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder;
    llvm::Module module;

    std::map<std::string, Symbol> symbolTable;
};

class ASTNode {
public:
    virtual ~ASTNode();
    //virtual void print(std::ostream& os, unsigned indent = 0) const = 0;
    //void gen() const;

    virtual llvm::Value* codegen(GenContext& gen) const = 0;
};


class TypeASTNode : public ASTNode {
public:
    enum class Type { INT,
        DOUBLE,
        Array,
        UNKNOWN
    };

    TypeASTNode(Type type, std :: optional<int> elemenets = {});
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
    llvm::Type* genType(GenContext& gen) const;
    Type getType() { return m_type; }

private:
    Type m_type;
    std :: optional<int> m_elements;
};


class ExprASTNode : public ASTNode {
public:
    virtual ~ExprASTNode();
};

class BinOpASTNode : public ExprASTNode {
    TokenType m_op;
    std::shared_ptr<ExprASTNode> m_lhs;
    std::shared_ptr<ExprASTNode> m_rhs;

public:
    BinOpASTNode(TokenType op, std::shared_ptr<ExprASTNode> lhs, std::shared_ptr<ExprASTNode> rhs);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;

};

class UnaryOpASTNode : public ExprASTNode {
    TokenType m_op;
    std::shared_ptr<ExprASTNode> m_expr;

public:
    UnaryOpASTNode(TokenType op, std::shared_ptr<ExprASTNode> expr);
   // void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};



class LiteralASTNode : public ExprASTNode {
    int64_t m_value;

public:
    LiteralASTNode(int64_t value);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

class DeclRefASTNode : public ExprASTNode {
    std::string m_var;
    std :: optional<int> m_index;
public:
    DeclRefASTNode(std::string var, std :: optional<int> index = {});
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
    llvm::AllocaInst* getStore(GenContext& gen) const;
};

class FunCallASTNode : public ExprASTNode {
    std::string m_func;
    std::vector<std::shared_ptr<ExprASTNode>> m_args;

public:
    FunCallASTNode(std::string func, std::vector<std::shared_ptr<ExprASTNode>> args);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};


class StatementASTNode : public ASTNode {
public:
    virtual ~StatementASTNode();
};

class IfASTNode : public StatementASTNode {
    std::shared_ptr<ExprASTNode> m_cond;
    std :: vector<std::shared_ptr<StatementASTNode>> m_body;
    std :: vector<std :: shared_ptr<StatementASTNode>> m_else;

public:
    IfASTNode(std::shared_ptr<ExprASTNode> cond,
              std :: vector<std::shared_ptr<StatementASTNode>> body,
              std :: vector<std :: shared_ptr<StatementASTNode>> Else);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

class ArrayASTNode : public StatementASTNode
{
    int m_offset;
    int m_size;
    std :: string m_name;

public :
    ArrayASTNode(int offset, int size, const std :: string & name);
    int getOffset() { return m_offset; }
    llvm::Value* codegen(GenContext& gen) const override;
};

class VarDeclASTNode : public StatementASTNode {
    std::string m_var;
    std::shared_ptr<TypeASTNode> m_type;
    std::shared_ptr<ExprASTNode> m_expr;

public:
    VarDeclASTNode(std::string var, std::shared_ptr<TypeASTNode> type, std::shared_ptr<ExprASTNode> expr);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

class WhileASTNode : public StatementASTNode
{
public:
    WhileASTNode(std :: shared_ptr<ExprASTNode> cond, std :: vector<std :: shared_ptr<StatementASTNode>> body);
    llvm::Value* codegen(GenContext& gen) const override;
private:
    std :: vector<std :: shared_ptr<StatementASTNode>> m_body;
    std :: shared_ptr<ExprASTNode> m_cond;
};

class ForAstNode : public StatementASTNode
{
public:
    enum Type
    {
        INC,
        DEC,
        UNKNOWN
    };
    ForAstNode(
            std :: shared_ptr<DeclRefASTNode> var,
            const Type & type,
            std :: shared_ptr<ExprASTNode> init,
            std :: shared_ptr<ExprASTNode> end,
            std :: vector<std :: shared_ptr<StatementASTNode>> body
            )
    : m_var(var), m_init(init), m_End(end), m_type(type), m_body(body) { }
    llvm::Value* codegen(GenContext& gen) const override;
private :
    mutable std :: shared_ptr<DeclRefASTNode> m_var;
    mutable std :: shared_ptr<ExprASTNode> m_init;
    mutable std :: shared_ptr<ExprASTNode> m_End;
    mutable std :: vector<std :: shared_ptr<StatementASTNode>> m_body;
    Type m_type;

};

class AssignASTNode : public StatementASTNode {
    std::shared_ptr<DeclRefASTNode> m_var;
    std::shared_ptr<ExprASTNode> m_expr;

public:
    AssignASTNode(std::shared_ptr<DeclRefASTNode> var, std::shared_ptr<ExprASTNode> expr);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};


class ExpressionStatementASTNode : public StatementASTNode {
    std::shared_ptr<ExprASTNode> m_expr;

public:
    ExpressionStatementASTNode(std::shared_ptr<ExprASTNode> expr);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
};

class ProgramASTNode : public ASTNode {
    std::vector<std::shared_ptr<StatementASTNode>> m_statements;

public:
    ProgramASTNode(std::vector<std::shared_ptr<StatementASTNode>> statements);
    //void print(std::ostream& os, unsigned indent = 0) const override;
    llvm::Value* codegen(GenContext& gen) const override;
    void insertStatement(std :: shared_ptr<StatementASTNode> && src) { m_statements.emplace_back(std :: move(src));}
};
