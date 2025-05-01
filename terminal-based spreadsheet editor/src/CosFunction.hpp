#pragma once

#include "UnaryFunction.hpp"
#include "CInt.hpp"
#include "CDouble.hpp"
#include <math.h>


/**
 * this class is wrapper of math function : cosinus. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of unary function node in ast
 */
class CosFunction : public UnaryFunction
{
public :
    CosFunction() = default;
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;

    std :: shared_ptr<UnaryFunction> clone()                         override { return std :: make_shared<CosFunction>(CosFunction()); }

    virtual std :: ostream & print(std :: ostream & os)              override { return os << "cos"; }

    bool isCycle (
                    std :: shared_ptr<ASTAbstractNode> val,
                    std :: unordered_set<std :: string>  src
                  )  override
    {
        return val->isCyclePresent(src);
    }
    virtual ~CosFunction() {}
};