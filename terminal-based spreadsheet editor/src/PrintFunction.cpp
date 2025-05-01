#include "PrintFunction.hpp"


bool PrintFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)
{
    return val->isCyclePresent(visited);
}

std :: shared_ptr<AbstractType> PrintFunction :: evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    if(src == nullptr)
        return nullptr;
    auto resEval = src->evaluate();
    if(resEval == nullptr)
        return nullptr;
    resEval->print(std :: cout );
    std :: cout << "\n";
    return resEval;
}