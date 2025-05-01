#pragma once

#include "UnaryFunction.hpp"
#include "CInt.hpp"
#include <math.h>


/**
 * this class is wrapper of math function : Absolute value. It helps to work with ast ( = abstract syntax tree).
 *  it uses as operator of Binary function node in ast
 */
class AbsFunction : public UnaryFunction
{
public :
    AbsFunction() = default;
    /**
    * \param src : represents object of node in ast
    * \return  value after evaluation
     **/
    std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> src )  override;
    std :: shared_ptr<UnaryFunction> clone() override;
    std :: ostream & print(std :: ostream & os) override;
    bool isCycle (std :: shared_ptr<ASTAbstractNode> val, std :: unordered_set<std :: string>  src)  override;

    virtual ~AbsFunction() {}
};