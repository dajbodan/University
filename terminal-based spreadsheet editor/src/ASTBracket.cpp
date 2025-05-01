#include "ASTBracket.hpp"

std :: shared_ptr<AbstractType> ASTBracket :: evaluate()
{
    return m_val->evaluate();
}

std :: ostream & ASTBracket :: print(std :: ostream & os)
{
    os << " ( ";
    m_val->print(os);
    os << " ) ";
    return os;
}

bool ASTBracket :: isCyclePresent(std :: unordered_set<std :: string>  visited)
{
    return m_val->isCyclePresent(visited);
}