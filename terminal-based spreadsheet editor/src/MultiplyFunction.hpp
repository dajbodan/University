#pragma once

#include "BinaryFunction.hpp"
#include "CInt.hpp"
#include "AbstractType.hpp"
#include <numeric>

/**
 * this class is wrapper of math function : multiply. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */
class MultiplyFunction : public BinaryFunction
{
public:
    MultiplyFunction() = default;

    std :: shared_ptr<AbstractType> evaluate(std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs ) override;

    std :: shared_ptr<BinaryFunction> clone() override { return std :: make_shared<MultiplyFunction>(*this); }

    bool isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                  std :: shared_ptr<ASTAbstractNode> rhs,
                  std :: unordered_set<std :: string> src) override;
    std :: ostream & print(std :: ostream & os) override;
};