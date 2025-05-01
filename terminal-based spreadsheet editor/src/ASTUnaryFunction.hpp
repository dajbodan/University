#pragma once

#include "ASTAbstractNode.hpp"
#include "UnaryFunction.hpp"



/**
 * this class represents node unary function in ast
 */
class ASTUnaryFunction : public ASTAbstractNode
{
public :
    /**
     *
     * @param val  is node of ast
     * @param oper operation that is applied on val
     */
    ASTUnaryFunction(std :: shared_ptr<ASTAbstractNode> val, std :: shared_ptr<UnaryFunction> oper)
            : m_val(val), m_oper(oper) { }

    std :: shared_ptr<AbstractType> evaluate() override;
    std :: ostream & print(std :: ostream & os) override;
    bool isCyclePresent(std :: unordered_set<std :: string>  visited ) override;

private:
    std :: shared_ptr<ASTAbstractNode> m_val;
    std :: shared_ptr<UnaryFunction> m_oper;
};