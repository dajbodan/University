#include "CToken.hpp"


CToken :: CToken(const int src, Type type)
{
    if(type != Type ::VARIABLE)
        throw std :: exception();
    m_intName = src;
    m_typeInName = IntType;
    m_type = type;
}

CToken :: Type CToken :: getTypeValue()
{
    if(m_typeInName == StringType)
        return Type :: STRING;
    else if(m_typeInName == IntType)
        return Type :: INT;
    return UNKNOWN;
}

int  CToken :: getIntNameToken()
{
    if(m_type == CToken :: VARIABLE)
        return m_intName;
    throw std :: exception();
}