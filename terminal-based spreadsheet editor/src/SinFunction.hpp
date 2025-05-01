#pragma once

#include "UnaryFunction.hpp"
#include <math.h>
#include "CDouble.hpp"


/**
 * this class is wrapper of math function : sinus. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */
class SinFunction : public UnaryFunction
{
public :
    SinFunction() = default;
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;
    std :: shared_ptr<UnaryFunction> clone() override { return std :: make_shared<SinFunction>(SinFunction()); }
    virtual std :: ostream & print(std :: ostream & os) override { return os << "sin"; }
    bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)  override
    {
        return val->isCyclePresent(visited);
    }
    ~SinFunction() {}
};