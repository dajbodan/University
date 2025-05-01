#include "Table.hpp"


bool Table :: isPresentInTable(const std :: string & srcName) const
{
    for(auto & it : m_Table)
            if(it->m_name == srcName)
                return true;
    return false;
}

std :: ostream & Table ::printTable(std :: ostream & os) const
{
    for(auto it : m_Table)
    {

            os << "Name : >" << it->m_name << "< , formula : ";
            it->m_start->print(os);
            os << ", Value : ";
            if(it->m_start->evaluate()->tryGetInt().has_value())
                os << it->m_start->evaluate()->tryGetInt().value();
            else if(it->m_start->evaluate()->tryGetString().has_value())
                os << it->m_start->evaluate()->tryGetString().value();
            else if(it->m_start->evaluate()->tryGetDouble().has_value())
                os << it->m_start->evaluate()->tryGetDouble().value();
            os << "\n";

    }
    return os;
}

std :: vector<std :: string> Table :: convertToFormula() const
{
    std :: vector<std :: string> res;
    for(auto & it : m_Table)
    {
        std :: ostringstream oss;
        it->m_start->print(oss);
        res.push_back(it->m_name + " = " + oss.str() + "\n");
    }
    return res;
}

std :: shared_ptr<Table :: WrapperNode>  Table :: getCell(const std :: string & idCell)
{
    if(!isPresentInTable(idCell))
        throw std :: exception();
    for(auto & it : m_Table)
            if(it->m_name == idCell)
                return it;
    throw std :: exception();
}




bool Table :: insertTable(const std :: string & nameCell, std :: shared_ptr<ASTAbstractNode> srcRoot)
{
    try {
        auto cell = getCell(nameCell);
        cell->m_start = srcRoot;
    }
    catch(...)
    {
        m_Table.push_back(std ::make_shared<WrapperNode>(WrapperNode(nameCell, srcRoot)));
    }
    return true;

}