#pragma once
#include "ASTAbstractNode.hpp"
#include "Table.hpp"
#include <iostream>



/**
 * this class represent link node in ast
 */
class ASTLinkNode : public ASTAbstractNode
{

    std :: shared_ptr<Table :: WrapperNode>  m_link;
public:
    ASTLinkNode(const std :: string & id, std :: shared_ptr<Table> table);
    std :: shared_ptr<AbstractType> evaluate() override;
    std :: ostream &  print(std :: ostream & os) override;
    bool isCyclePresent(std :: unordered_set<std :: string> ) override;
};