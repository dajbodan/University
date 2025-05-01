#pragma once


#include <iostream>
#include <memory>
#include <optional>
#include "IPrintable.hpp"

/**
 * This class represents value that program can return
 *
 */
class AbstractType : public IPrintable
{
public :
    virtual std :: optional<std :: string> tryGetString()  const = 0;
    virtual std :: optional<int>           tryGetInt   () const  = 0;
    virtual std :: optional<double>        tryGetDouble () const = 0;
    virtual ~AbstractType()  = default;
};