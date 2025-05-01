#include "MultiplyFunction.hpp"

std :: ostream & MultiplyFunction :: print(std :: ostream & os)
{
    return os << "*";
}

std :: shared_ptr<AbstractType> MultiplyFunction :: evaluate(std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs )
{
    auto lhsRes = lhs->evaluate();
    auto rhsRes = rhs->evaluate();
    if(lhsRes != nullptr && rhsRes != nullptr && lhsRes->tryGetInt() && rhsRes->tryGetInt())
    {
        return std::make_shared<CInt>(CInt(lhsRes->tryGetInt().value() * rhsRes->tryGetInt().value()));
    }
    if(lhsRes != nullptr && rhsRes != nullptr && lhsRes->tryGetString() && rhsRes->tryGetInt())
    {
        std :: string tempRes = "";
        for(int i = 0; i < rhsRes->tryGetInt(); i++)
            tempRes += lhsRes->tryGetString().value();
        return std ::make_shared<CString>(CString(tempRes));
    }
    return nullptr;
}

bool MultiplyFunction :: isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                               std :: shared_ptr<ASTAbstractNode> rhs,
                               std :: unordered_set<std :: string> src)
{
    return lhs->isCyclePresent(src) || rhs->isCyclePresent(src);
}