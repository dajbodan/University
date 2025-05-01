#pragma once

#include "AbstractType.hpp"
#include "CString.hpp"
#include <optional>


/**
 * this class is wrapper of data type int
 */
class CInt : public AbstractType
{
public :
    CInt(int srcVal) : m_val(srcVal) { }

    std :: optional<std :: string>  tryGetString() const   override     { return {}; }
    std :: optional<int> tryGetInt()               const   override     { return m_val; }
    std :: optional<double>        tryGetDouble () const   override     { return {}; }
    std :: ostream & print(std :: ostream & os) override                { return os << m_val; }

private :
    int m_val;
};