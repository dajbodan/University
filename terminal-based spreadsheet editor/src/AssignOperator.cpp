#include "AssignOperator.hpp"


std :: shared_ptr<AbstractType> AssignOperator :: evaluate( std :: shared_ptr<ASTAbstractNode> lhs,  std :: shared_ptr<ASTAbstractNode> rhs )
{
    auto name = lhs->evaluate()->tryGetString();
    auto tempResRhs = rhs->evaluate();
    if(tempResRhs == nullptr || ! name.has_value())
        return nullptr;
    m_table->insertTable(*name, rhs);
    return tempResRhs;
}

std :: ostream & AssignOperator :: print(std :: ostream & os)
{
    return os << "=";
}

 bool AssignOperator :: isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                                        std :: shared_ptr<ASTAbstractNode> rhs,
                                        std :: unordered_set<std :: string>  visited)
{
    visited.insert(lhs->evaluate()->tryGetString().value());
    return rhs->isCyclePresent(visited);
}