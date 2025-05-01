#include "ASTLeafNode.hpp"

std :: shared_ptr<AbstractType> ASTLeafNode :: evaluate()
{
    return m_val;
}

std :: ostream  & ASTLeafNode :: print(std :: ostream & os)
{
    return m_val->print(os);
}

bool ASTLeafNode :: isCyclePresent(std :: unordered_set<std :: string> visited )
{
    return false;
}