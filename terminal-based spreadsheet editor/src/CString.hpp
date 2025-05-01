#pragma once

#include "AbstractType.hpp"
#include <optional>

/**
 * this class is wrapper of std :: string type
 */
class CString : public AbstractType
{
public :
    CString(const std :: string & srcVal) : m_val(srcVal) { }


    std :: optional<std :: string> tryGetString()  const  override     { return std :: optional(m_val); }
    std :: optional<int> tryGetInt()               const  override     { return {}; }
    std :: optional<double> tryGetDouble()         const override { return {}; }
    std :: ostream & print(std :: ostream & os) override { return os << m_val; }

private :
    std :: string m_val;
};