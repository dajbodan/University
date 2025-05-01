#pragma once

#include "ASTAbstractNode.hpp"
#include "BinaryFunction.hpp"

/**
 * this class represents bracket node in ast
 */
class ASTBracket : public ASTAbstractNode
{
public :


    ASTBracket(std :: shared_ptr<ASTAbstractNode> src) : m_val(src) { }

    std :: shared_ptr<AbstractType> evaluate() override;
    std :: ostream & print(std :: ostream & os) override;
    bool isCyclePresent(std :: unordered_set<std :: string>  visited) override;

private:
    std :: shared_ptr<ASTAbstractNode> m_val;
};