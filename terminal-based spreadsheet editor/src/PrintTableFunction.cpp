#include "PrintTableFunction.hpp"

std :: shared_ptr<AbstractType> PrintTableFunction ::  evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    m_table->printTable(std :: cout);
    return std ::make_shared<CDataDummyValue>(CDataDummyValue());
}


bool PrintTableFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)
{
    return false;
}