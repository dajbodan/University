#pragma once

#include "AbstractType.hpp"
#include <optional>
#include "ASTAbstractNode.hpp"
#include "IPrintable.hpp"
#include "ICycleDetectable.hpp"


/**
 * this abstract class represents BinaryFunction
 */
class BinaryFunction : public IPrintable
{
public :
    /**
     *
     * @param lhs node of ast
     * @param rhs node of ast
     * @return result of binary operation or nullptr when something goes wrong
     */
    virtual std :: shared_ptr<AbstractType> evaluate( std :: shared_ptr<ASTAbstractNode> lhs,  std :: shared_ptr<ASTAbstractNode> rhs ) = 0;
    virtual bool isCycle (std :: shared_ptr<ASTAbstractNode> lhs, std :: shared_ptr<ASTAbstractNode> rhs, std :: unordered_set<std :: string>  src) = 0;
    virtual std :: shared_ptr<BinaryFunction> clone() = 0;

    virtual ~BinaryFunction() {}
};