#include "ast.hpp"

ASTNode::~ASTNode() = default;

TypeASTNode::TypeASTNode(Type type, std :: optional<int> elements)
        : m_type(type), m_elements(elements)
{
}

ExprASTNode::~ExprASTNode() = default;

BinOpASTNode::BinOpASTNode(TokenType op, std::shared_ptr<ExprASTNode> lhs, std::shared_ptr<ExprASTNode> rhs)
        : m_op(op)
        , m_lhs(std::move(lhs))
        , m_rhs(std::move(rhs))
{
}

UnaryOpASTNode::UnaryOpASTNode(TokenType op, std::shared_ptr<ExprASTNode> expr)
        : m_op(op)
        , m_expr(std::move(expr))
{
}

LiteralASTNode::LiteralASTNode(int64_t value)
        : m_value(value)
{
}

DeclRefASTNode::DeclRefASTNode(std::string var, std :: optional<int> index)
        : m_var(std::move(var)), m_index(index)
{
}

FunCallASTNode::FunCallASTNode(std::string func, std::vector<std::shared_ptr<ExprASTNode>> args)
        : m_func(std::move(func))
        , m_args(std::move(args))
{
}

StatementASTNode::~StatementASTNode() = default;

IfASTNode :: IfASTNode(std::shared_ptr<ExprASTNode> cond,
std :: vector<std::shared_ptr<StatementASTNode>> body,
        std :: vector<std :: shared_ptr<StatementASTNode>> Else)
        : m_cond(cond), m_body(body), m_else(Else)
{

}


VarDeclASTNode::VarDeclASTNode(std::string var, std::shared_ptr<TypeASTNode> type, std::shared_ptr<ExprASTNode> expr)
        : m_var(std::move(var))
        , m_type(std::move(type))
        , m_expr(std::move(expr))
{
}

AssignASTNode::AssignASTNode(std::shared_ptr<DeclRefASTNode> var, std::shared_ptr<ExprASTNode> expr)
        : m_var(std::move(var))
        , m_expr(std::move(expr))
{
}

ExpressionStatementASTNode::ExpressionStatementASTNode(std::shared_ptr<ExprASTNode> expr)
        : m_expr(std::move(expr))
{
}

ProgramASTNode::ProgramASTNode(std::vector<std::shared_ptr<StatementASTNode>> statements)
        : m_statements(std::move(statements))
{
}

ArrayASTNode :: ArrayASTNode(int offset, int size, const std :: string & name) :
    m_offset(offset), m_size(size), m_name(name)
{

}


WhileASTNode :: WhileASTNode(std :: shared_ptr<ExprASTNode> cond, std :: vector<std :: shared_ptr<StatementASTNode>> body)
: m_cond(cond), m_body(body)
{

}

