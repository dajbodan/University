#pragma once

#include "UnaryFunction.hpp"
#include "CDataDummyValue.hpp"
#include <functional>
#include <math.h>


/**
 * this class is wrapper of loading statements. It It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of unary function node in ast
 */
class LoadFunction : public UnaryFunction
{
public :
    LoadFunction(std :: function<void(const std :: string &)> loader) : m_loader(loader) { }
    /**
     * calls m_loader in order to change the way of loading statements. When it cannot call, it failse
     * @param src
     * @return dummy value
     */
    std :: shared_ptr<AbstractType>  evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;
    std :: shared_ptr<UnaryFunction> clone()                                             override;
    virtual std :: ostream &         print(std :: ostream & os) override { return os << "load"; }
    bool                             isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  src)  override;
    virtual ~LoadFunction() {}
private :
    std :: function<void(const std :: string &)> m_loader;
};