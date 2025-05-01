#include "DivideFunction.hpp"


std :: ostream & DivideFunction :: print(std :: ostream & os)
{
    return os << "/";
}

std :: shared_ptr<AbstractType> DivideFunction :: evaluate(std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs )
{
    auto lhsRes = lhs->evaluate();
    auto rhsRes = rhs->evaluate();
    if(lhsRes != nullptr && rhsRes != nullptr && lhsRes->tryGetInt() && rhsRes->tryGetInt() && rhsRes->tryGetInt().value() != 0)
        return std ::make_shared<CInt>(CInt(lhsRes->tryGetInt().value() / rhsRes->tryGetInt().value()));
    return nullptr;
}