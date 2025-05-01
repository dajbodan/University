#include "CLexan.hpp"

bool CLexan ::  readInput(const std :: string & src)
{

    std :: queue<std :: string> que = parseInputTokens(src);
    while(! que.empty())
    {
        std :: string inputStr = que.front();
        que.pop();
        if (isSin(inputStr));
        else if (isCos(inputStr));
        else if (isAbs(inputStr));
        else if (isPrint(inputStr));
        else if (isSave(inputStr));
        else if (isLoad(inputStr));
        else if (isPlus(inputStr));
        else if (isMinus(inputStr));
        else if (isStar(inputStr));
        else if (isSlash(inputStr));
        else if (isDot(inputStr));
        else if (isLeftRoundBracket(inputStr));
        else if (isRightRoundBracket(inputStr));
        else if (isAssign(inputStr));
        else if (isVariable(inputStr));
        else { return false; }
    }

    return true;
}


std :: queue<std :: string> CLexan :: parseInputTokens(const std :: string & src)
{
    std :: queue<std :: string> res;
    std :: istringstream iss(src);
    std :: string temp;
    while(iss >> temp)
        res.push(temp);
    return res;
}

bool CLexan ::  isCos(const std :: string & src)
{
    if(src.size() == 3 and src == "cos")
    {
        m_tokens.push(CToken("cos"));
        return true;
    }
    return false;
}

bool CLexan :: isPrint(const std :: string & src)
{
    if(src.size() == 5 and src.substr(0, 5) == "print" )
    {
        m_tokens.push(CToken("print"));
        return true;
    }
    return false;
}

bool CLexan :: isSin(const std :: string & src)
{
    if(src.size() == 3 and src.substr(0, 3) == "sin" )
    {
        m_tokens.push(CToken("sin"));
        return true;
    }
    return false;
}


bool CLexan :: isAbs(const std :: string & src)
{
    if(src.size() == 3 and src == "abs" )
    {
        m_tokens.push(CToken("abs"));
        return true;
    }
    return false;
}

bool CLexan :: isSave(const std :: string & src)
{
    if(src.size() == 4 and src == "save" )
    {
        m_tokens.push(CToken("save"));
        return true;
    }
    return false;
}

bool CLexan :: isLoad(const std :: string & src)
{
    if(src.size() == 4 and src.substr(0, 4) == "load")
    {
        m_tokens.push(CToken("load"));
        return true;
    }
    return false;
}


bool CLexan :: isPlus(const std :: string & src)
{
    if(src.size() == 1 && src == "+")
    {
        m_tokens.push(CToken("+"));
        return true;
    }
    return false;
}

bool CLexan :: isMinus(const std :: string & src)
{
    if(src.size() == 1 && src == "-" )
    {
        m_tokens.push(CToken("-"));
        return true;
    }
    return false;
}

bool CLexan ::isLeftRoundBracket(const std::string & src)
{
    if(src.size() == 1 && src == "(" )
    {
        m_tokens.push(CToken("("));
        return true;
    }
    return false;
}


bool CLexan ::isRightRoundBracket(const std::string & src)
{
    if(src.size() == 1 && src == ")")
    {
        m_tokens.push(CToken(")"));
        return true;
    }
    return false;
}


bool CLexan ::isAssign(const std::string & src)
{
    if(src.size() == 1 && src == "=")
    {
        m_tokens.push(CToken("="));
        return true;
    }
    return false;
}

bool CLexan :: isPrintTable(const std :: string & src)
{
    if(src.size() == 10 and src == "PrintTable")
    {
        m_tokens.push(CToken("PrintTable"));
        return true;
    }
    return false;
}

bool CLexan ::  isStar(const std :: string & src)
{
    if(src.size() == 1 && src == "*")
    {
        m_tokens.push(CToken("*"));
        return true;
    }
    return false;
}

bool CLexan ::  isDot(const std :: string & src)
{
    if(src.size() == 1 and src.substr(0, 1) == ".")
    {
        m_tokens.push(CToken("."));
        return true;
    }
    return false;
}

bool CLexan ::  isSlash(const std :: string & src)
{
    if(src.size() > 0 && src.substr(0, 1) == "/" )
    {
        m_tokens.push(CToken("/"));

        return true;
    }
    return false;
}


bool CLexan :: isVariable(const std :: string & src)
{
    std :: unordered_set<std :: string> specialSigns;
    specialSigns.insert("+");
    specialSigns.insert("-");
    specialSigns.insert("*");
    specialSigns.insert(".");
    specialSigns.insert("/");
    std :: string variableStr = "";
    int variableInt = -1;
    for(auto & it : src)
    {
        if ( std::isdigit(it)and variableStr == "")
            variableInt = variableInt == -1 ? it - '0' : variableInt * 10 + it - '0';
        else if ((specialSigns.find(std :: string(1, it)) != specialSigns.end() or std::isalpha(it)) and variableInt == -1)
            variableStr += it;
        else
            return false;
    }

    if (variableStr != "")
        m_tokens.push(CToken(variableStr, CToken::VARIABLE));
    else if (variableInt != -1)
        m_tokens.push(CToken(variableInt, CToken::VARIABLE));
    else
        return false;
    return true;
}


CToken CLexan :: getNextToken()
{
    auto top = m_tokens.front();
    return top;
}

