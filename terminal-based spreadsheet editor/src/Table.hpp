#pragma once

#include <vector>
#include <memory>
#include "AbstractType.hpp"
#include "ASTAbstractNode.hpp"
#include <sstream>
#include <numeric>
/**
 * this class contains all records about formulas.
 */
class Table
{
public :
    Table() = default;

    bool isPresentInTable(const std :: string & srcName) const;

    bool insertTable(const std :: string & nameCell, std :: shared_ptr<ASTAbstractNode> srcRoot);

    std :: ostream & printTable(std :: ostream & ) const;
    /**
     *  this method take formulas from table and put it into vector
     * @return vector of fomulas
     */
    std :: vector<std :: string> convertToFormula() const ;
private :
    struct WrapperNode;

    std :: shared_ptr<Table :: WrapperNode>  getCell(const std :: string & idCell);

        friend class ASTLinkNode;
        struct WrapperNode
        {
            WrapperNode(const std :: string & name, std :: shared_ptr<ASTAbstractNode> start ) : m_name(name), m_start(start) {}
            std :: string m_name;
            std :: shared_ptr<ASTAbstractNode> m_start;
        };
    std :: vector<std :: shared_ptr<WrapperNode>> m_Table; // maybe set is better
};