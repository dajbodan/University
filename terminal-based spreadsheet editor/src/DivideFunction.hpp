#pragma once

#include "BinaryFunction.hpp"
#include "CInt.hpp"
#include "AbstractType.hpp"


/**
 * this class is wrapper of math function : divide. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */
class DivideFunction : public BinaryFunction
{
public:
    DivideFunction() = default;

    std :: shared_ptr<AbstractType> evaluate(std :: shared_ptr<ASTAbstractNode> lhs,
                                             std :: shared_ptr<ASTAbstractNode> rhs ) override;

    std :: shared_ptr<BinaryFunction> clone() override { return std :: make_shared<DivideFunction>(*this); }

    bool isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                  std :: shared_ptr<ASTAbstractNode> rhs,
                  std :: unordered_set<std :: string> src) override
    {
        return lhs->isCyclePresent(src) || rhs->isCyclePresent(src);
    }
    std :: ostream & print(std :: ostream & os) override;
};