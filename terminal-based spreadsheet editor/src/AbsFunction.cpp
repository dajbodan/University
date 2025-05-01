#include "AbsFunction.hpp"

std :: shared_ptr<AbstractType> AbsFunction :: evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    auto tempRes = src->evaluate();
    if(! tempRes->tryGetInt().has_value())
        return nullptr;
    return std ::make_shared<CInt>(CInt(abs(tempRes->tryGetInt().value())));
}

std :: shared_ptr<UnaryFunction> AbsFunction :: clone()
{
    return std :: make_shared<AbsFunction>(AbsFunction());
}

std :: ostream & AbsFunction :: print(std :: ostream & os)
{
    return os << "abs";
}

bool AbsFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  src)
{
    return val->isCyclePresent(src);
}