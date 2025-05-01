#pragma once

#include "UnaryFunction.hpp"
#include <math.h>

class PrintFunction : public UnaryFunction
{
public :
    PrintFunction() = default;
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;

    std :: shared_ptr<UnaryFunction> clone() override { return std :: make_shared<PrintFunction>(PrintFunction()); }

    bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)  override;

    virtual std :: ostream & print(std :: ostream & os) override { return os << "print"; }
    virtual ~PrintFunction() {}
};