#include "ASTBinaryNode.hpp"


std :: shared_ptr<AbstractType> ASTBinaryNode ::evaluate()
{
    return m_oper->evaluate(m_left, m_right);
}


bool ASTBinaryNode :: isCyclePresent(std :: unordered_set<std :: string>  visited)
{
    return m_oper->isCycle(m_left, m_right, visited);
}

std :: ostream & ASTBinaryNode :: print(std :: ostream & os)
{
    m_left->print(os) << " ";
    m_oper->print(os) << " ";
    m_right->print(os);
    return os;
}