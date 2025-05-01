#pragma once

#include "AbstractType.hpp"
#include <optional>


/**
 * this is dummy class. This class is returned instead of return nullptr. Returning nullptr in method evaluate()
 *  means something wrong. In some operation it's difficult to return
 * not dummy value and at the same time not to return nullptr ( example : SaveFileFunction :: evaluate())
 */
class CDataDummyValue : public AbstractType
{
public :
    CDataDummyValue() = default;
    std :: optional<std :: string>  tryGetString()   const            override              { return  {}; }
    std :: optional<int> tryGetInt()                 const            override              { return  {}; }
    std :: optional<double> tryGetDouble()           const            override              { return  {}; }
    std :: ostream & print(std :: ostream & os)                       override              { return os << "Dummmy value"; }


};