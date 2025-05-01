#include "PlusFunction.hpp"


std :: ostream & PlusFunction :: print(std :: ostream & os)
{
    return os << "+";
}

std :: shared_ptr<AbstractType> PlusFunction :: evaluate(std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs )
{
    auto lhsRes = lhs->evaluate();
    auto rhsRes = rhs->evaluate();
    if(lhsRes != nullptr && rhsRes != nullptr && lhsRes->tryGetInt() && rhsRes->tryGetInt())
        return std ::make_shared<CInt>(CInt(lhsRes->tryGetInt().value() + rhsRes->tryGetInt().value()));
    return nullptr;
}


bool PlusFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                               std :: shared_ptr<ASTAbstractNode> rhs,
                               std :: unordered_set<std :: string> src)
{
    return lhs->isCyclePresent(src) || rhs->isCyclePresent(src);
}