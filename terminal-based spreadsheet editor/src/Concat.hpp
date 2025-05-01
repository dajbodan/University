#pragma once


#include "BinaryFunction.hpp"
#include "Table.hpp"
#include "CString.hpp"

/**
 * this class is wrapper of string function : concatenation. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */

class Concat : public BinaryFunction
{
public :
    Concat() = default;
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> lhs,
                                              std :: shared_ptr<ASTAbstractNode> rhs ) override;
    std :: shared_ptr<BinaryFunction> clone() override { return std ::make_shared<Concat>(*this); }
     bool isCycle (std :: shared_ptr<ASTAbstractNode> lhs,
                   std :: shared_ptr<ASTAbstractNode> rhs,
                   std :: unordered_set<std :: string> src) override;
    std :: ostream & print(std :: ostream & os) override;
};