#include "AbstractSyntaxTree.hpp"

std :: shared_ptr<AbstractType> AbstractSyntaxTree :: evaluate()
{
    if(m_root == nullptr)
        return nullptr;
    return m_root->evaluate();
}


std::ostream & AbstractSyntaxTree ::  print(std::ostream &os)
{
    return m_root->print(os);
}

bool AbstractSyntaxTree :: isCycleDetected()
{
    std :: unordered_set<std :: string> visits;
    return m_root->isCyclePresent(visits);
}
