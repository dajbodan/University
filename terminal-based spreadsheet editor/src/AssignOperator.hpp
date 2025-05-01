#pragma once

#include "BinaryFunction.hpp"
#include "Table.hpp"


/**
 * this class is wrapper of function : assign what is on right hand side to left side.
 *  left must be string type. When variable on left side is not declare in Table, then it creates a new one
 *  and push it in Table.
 *  It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */
class AssignOperator : public BinaryFunction
{
public :
    AssignOperator(std :: shared_ptr<Table> table) : m_table(table) { }

    /**
     *
     * @param lhs represents ast node which is on the left. It's variable of string type. Example : A = 5 -> left  : A
     * @param rhs represent ast node which is on the right. It's formula.                 Example : A = 5 -> right : 5
     * @return value which is on rhs
     */
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> lhs,  std :: shared_ptr<ASTAbstractNode> rhs ) override;

    std :: shared_ptr<BinaryFunction> clone() override { return std ::make_shared<AssignOperator>(*this); }

    /**
     *  it inserts variable on the left side in visited
     * @param lhs represents ast node which is on left side
     * @param rhs represent ast node which is on right side
     * @param visited repesent link nodes which were visited before
     * @return wether there is cycle of link nodes or not
     */
    virtual bool isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                          std :: shared_ptr<ASTAbstractNode> rhs,
                          std :: unordered_set<std :: string>  visited) override;

    std :: ostream & print(std :: ostream & os) override;
private :
    std :: shared_ptr<Table> m_table;
};