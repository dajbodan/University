#include "ASTUnaryFunction.hpp"

std :: shared_ptr<AbstractType> ASTUnaryFunction :: evaluate()
{
    return m_oper->evaluate(m_val);
}

std :: ostream & ASTUnaryFunction :: print(std :: ostream & os)
{
    m_oper->print(os);
    os << " ( ";
    m_val->print(os);
    os << " ) ";
    return os;
}


bool ASTUnaryFunction :: isCyclePresent(std :: unordered_set<std :: string>  visited )
{
    return m_oper->isCycle(m_val, visited);
}