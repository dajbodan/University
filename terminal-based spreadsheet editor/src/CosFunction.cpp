#include "CosFunction.hpp"

std :: shared_ptr<AbstractType> CosFunction :: evaluate( std :: shared_ptr<ASTAbstractNode> src )
{

    auto tempRes = src->evaluate();
    if(! tempRes->tryGetInt().has_value())
            return nullptr;
    return std ::make_shared<CDouble>(cos( tempRes->tryGetInt().value()));
}