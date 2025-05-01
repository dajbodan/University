#pragma once

#include "AbstractType.hpp"
#include <optional>
#include "ASTAbstractNode.hpp"
#include "IPrintable.hpp"


/**
 * this abstract class represents unary function.
 */
class UnaryFunction : public IPrintable
{
public :
    virtual std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src ) = 0;
    virtual bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  src) = 0;
    virtual std :: shared_ptr<UnaryFunction> clone() = 0;
    virtual ~UnaryFunction() {}

};