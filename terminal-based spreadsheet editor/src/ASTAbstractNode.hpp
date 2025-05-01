#pragma once

#include "AbstractType.hpp"
#include <unordered_set>


/**
 * this represents polymorphic class. It uses as node in ast
 */
class ASTAbstractNode
{
public:
    /**
     * this method evaluates each node in ast
     * @return internal value of program
     */
    virtual std :: shared_ptr<AbstractType> evaluate() = 0;
    /**
     *
     * @param os reprsent output
     * @return output with inserted value
     */
    virtual std :: ostream &  print(std :: ostream & os) = 0;
    /**
     *
     * @return wehter there is cycle or not
     */
    virtual bool isCyclePresent(std :: unordered_set<std :: string>  ) = 0;
    virtual ~ASTAbstractNode() { }
};