#pragma once


#include "AbstractType.hpp"
#include "IPrintable.hpp"
#include "ICycleDetectable.hpp"
#include "CInt.hpp"
#include "CString.hpp"
#include "ASTAbstractNode.hpp"
#include <optional>
#include <memory>
#include <unordered_set>
#include <iostream>


/**
 * This class represents formula which is saved in Table
 */
class AbstractSyntaxTree : public IPrintable, public ICycleDetectable
{
public :
    AbstractSyntaxTree( ) : m_root(nullptr) {}
    /**
     * @return nullptr signalizes that something is wrong. Otherwise, it returns evaluated value, which has type AbstractType
     */
    std :: shared_ptr<AbstractType> evaluate();

    std::ostream & print(std::ostream &os) override;
    /**
     * this method is used to detect cycle
     * example when it detects :
     *  A = 20
     *  B = A
     *  A = B
     * @return wether there is cycle in link nodes or not
     */
    bool isCycleDetected() override;

private :
    friend class CParser;

    std :: shared_ptr<ASTAbstractNode> m_root;
};