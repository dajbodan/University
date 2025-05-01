#include "ASTLinkNode.hpp"

ASTLinkNode :: ASTLinkNode(const std :: string & id, std :: shared_ptr<Table> table) : m_link(std ::make_shared<Table :: WrapperNode>(Table :: WrapperNode("", nullptr)))
{
    try {
        m_link = table->getCell(id);
    }
    catch(...)
    {
        std :: cout << "Exception\n"; // FIXME
    }
}

std :: shared_ptr<AbstractType> ASTLinkNode :: evaluate()
{
    return m_link->m_start->evaluate();
}

std :: ostream &  ASTLinkNode :: print(std :: ostream & os)
{
    return m_link->m_start->print(os);
}


bool ASTLinkNode :: isCyclePresent(std :: unordered_set<std :: string>  visited)
{
    if(visited.find(m_link->m_name) != visited.end())
        return true;
    visited.insert(m_link->m_name);
    return m_link->m_start->isCyclePresent(visited);
}