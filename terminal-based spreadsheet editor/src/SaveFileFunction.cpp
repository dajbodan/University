#include "SaveFileFunction.hpp"


std :: shared_ptr<AbstractType> SaveFileFunction :: evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    auto path = src->evaluate();
    if(path == nullptr || !path->tryGetString().has_value())
        return nullptr;

    try{ FileSaver saver(path->tryGetString().value()); saver.save(m_table); }
    catch (std :: exception & ex) { std :: cerr << "cannot save\n"; }

    return std ::make_shared<CDataDummyValue>(CDataDummyValue());
}

bool SaveFileFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)
{
    return false;
}