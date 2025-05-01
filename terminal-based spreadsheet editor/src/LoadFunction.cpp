#include "LoadFunction.hpp"

std :: shared_ptr<AbstractType> LoadFunction ::  evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    auto temp = src->evaluate();
    if(temp != nullptr && temp->tryGetString().has_value())
    {
        m_loader(src->evaluate()->tryGetString().value());
        return std ::make_shared<CDataDummyValue>(CDataDummyValue());
    }
    return nullptr;
}

std :: shared_ptr<UnaryFunction> LoadFunction :: clone()
{
    return std :: make_shared<LoadFunction>(LoadFunction( m_loader));
}

bool LoadFunction ::  isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  src)
{
    return false;
}