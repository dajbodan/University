#pragma once

#include "UnaryFunction.hpp"
#include "FileSaver.hpp"
#include "CDataDummyValue.hpp"
#include <math.h>


/**
 * this class is wrapper of file function.  It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of unary function node in ast
 */

class SaveFileFunction : public UnaryFunction
{
public :
    SaveFileFunction(std :: shared_ptr<Table> table) : m_table(table) { }
    /**
     * it saves into file. Otherwise it prints on cerr mistake
     * @param src
     * @return
     */
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;

    std :: shared_ptr<UnaryFunction> clone() override { return std :: make_shared<SaveFileFunction>(SaveFileFunction(m_table)); }

    virtual std :: ostream & print(std :: ostream & os) override { return os << "save"; }

    bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  visited)  override;
    ~SaveFileFunction() {}

private:

    std ::shared_ptr<Table> m_table;
};