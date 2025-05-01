#pragma once

#include "AbstractType.hpp"
#include <optional>


/**
 * this class is wrapper of data type : double
 */
class CDouble : public AbstractType
{
public :
    CDouble(double srcVal) : m_val(srcVal) { }

    std :: optional<std :: string>  tryGetString()   const override          { return {}; }
    std :: optional<int> tryGetInt()                 const override          { return {}; }
    std :: optional<double> tryGetDouble()           const override          { return m_val; }
    std :: ostream & print(std :: ostream & os) override                     { return os << m_val; }

private :
    double m_val;
};