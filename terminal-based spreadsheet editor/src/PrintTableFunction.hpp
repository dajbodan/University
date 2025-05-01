#pragma once

#include "UnaryFunction.hpp"
#include "CDataDummyValue.hpp"
#include "Table.hpp"
#include <math.h>

/**
 * this class is wrapper of output stream .  It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of unary function node in ast
 */
class PrintTableFunction : public UnaryFunction
{
public :
    PrintTableFunction(std :: shared_ptr<Table> table) : m_table(table) { }
    /**
     * it prints a table into std :: cout
     * @param src
     * @return
     */
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;

    std :: shared_ptr<UnaryFunction> clone()            override { return std :: make_shared<PrintTableFunction>(PrintTableFunction(m_table)); }
    virtual std :: ostream & print(std :: ostream & os) override { return os << "printTable"; }
    bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)  override;

    ~PrintTableFunction() {}

private:
    std :: shared_ptr<Table> m_table;
};