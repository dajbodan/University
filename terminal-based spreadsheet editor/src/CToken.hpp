#pragma  once

#include <iostream>


/**
 * this class represents tokens which is used lexical analyse
 */
class CToken
{

public :
    enum Type {
        VARIABLE, UNKNOWN, STRING, INT
    };


    CToken(const std :: string & src, Type type = Type :: UNKNOWN) : m_strName(src), m_typeInName(StringType), m_type(type)  { }
    CToken(const int src, Type type);

    bool isEqual        (const std :: string & src) const { return src == m_strName; }

    bool isTypeVariable(const CToken :: Type & src) const { return m_type == src; }

    std :: string getStrNameToken()                       { return m_strName; }

    Type          getTypeValue();
    int           getIntNameToken();

private :
    static const int StringType = 1;
    static const int IntType    = 0;
    std :: string m_strName;
    int           m_intName;
    int           m_typeInName;
    Type m_type;
};