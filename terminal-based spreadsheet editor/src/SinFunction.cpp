#include "SinFunction.hpp"
std :: shared_ptr<AbstractType> SinFunction :: evaluate( std :: shared_ptr<ASTAbstractNode> src )
{
    auto tempRes = src->evaluate();
    if(! tempRes->tryGetInt().has_value())
        return nullptr;

    return std ::make_shared<CDouble>(sin( tempRes->tryGetInt().value()));
}