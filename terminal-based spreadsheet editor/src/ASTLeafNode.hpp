#pragma once

#include "ASTAbstractNode.hpp"


/**
 * this class represents leaf node in ast
 */
class ASTLeafNode : public ASTAbstractNode
{
public :
    ASTLeafNode(std :: shared_ptr<AbstractType> val) : m_val(val) { }

    std :: shared_ptr<AbstractType> evaluate() override;
    std :: ostream  & print(std :: ostream & os) override;
    bool isCyclePresent(std :: unordered_set<std :: string> visited ) override;

private:
    std :: shared_ptr<AbstractType> m_val;
};