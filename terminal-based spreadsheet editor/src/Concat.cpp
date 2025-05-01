#include "Concat.hpp"


std :: ostream & Concat :: print(std :: ostream & os)
{
    return os << ".";
}

std :: shared_ptr<AbstractType> Concat :: evaluate(std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs )
{
    auto lhsRes = lhs->evaluate();
    auto rhsRes = rhs->evaluate();
    if(lhsRes != nullptr && rhsRes != nullptr && lhsRes->tryGetString() && rhsRes->tryGetString())
        return std ::make_shared<CString>(CString(lhsRes->tryGetString().value().append(rhsRes->tryGetString().value())));
    return nullptr;
}


bool Concat :: isCycle (std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs, std :: unordered_set<std :: string>  src)
{
    return lhs->isCyclePresent(src) || rhs->isCyclePresent(src);
}