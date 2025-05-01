#pragma once

#include "ASTAbstractNode.hpp"
#include "BinaryFunction.hpp"

/**
 * this class represents node binary function in ast
 */
class ASTBinaryNode : public ASTAbstractNode
{

    std :: shared_ptr<ASTAbstractNode> m_left;
    std :: shared_ptr<ASTAbstractNode> m_right;
    std :: shared_ptr<BinaryFunction> m_oper;
public :
    /**
     *
     * @param left is node of ast, left operand
     * @param right is node of ast, right operand
     * @param oper operation that is applied on left and right operands
     */
    ASTBinaryNode(std :: shared_ptr<ASTAbstractNode> left, std :: shared_ptr<ASTAbstractNode> right, std :: shared_ptr<BinaryFunction> oper)
            : m_left(left), m_right(right), m_oper(oper) { }

   /**
    * @return  value after operation
    */
    std :: shared_ptr<AbstractType> evaluate() override;
    std :: ostream & print(std :: ostream & os) override;
    bool isCyclePresent(std :: unordered_set<std :: string>  visited) override;
};