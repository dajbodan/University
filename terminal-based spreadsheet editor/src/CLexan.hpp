#pragma once

#include <iostream>
#include "CToken.hpp"
#include <queue>
#include <cctype>
#include <sstream>
#include <queue>
#include <unordered_set>


/**
 * this class represents lexical analyze
 */
class CLexan
{
public :
    CLexan() = default;
    /**
     * this read string and parse it into tokens ( values that recognizes )
     * @param inputStr from input
     * @return whether everyting from inputStr knows
     */
    bool readInput(const std :: string & inputStr);


    CToken getNextToken();
    void popToken()       { m_tokens.pop(); }
    bool isEmpty()        { return m_tokens.empty(); }
private :
    /**
     * is string cos token
     * @return
     */
    bool isCos(const std :: string &);
    /**
     * is string sin token
     * @return
     */
    bool isSin(const std :: string &);
    /**
     * is string abs token
     * @return
     */
    bool isAbs(const std :: string &);
    /**
     * is string print token
     * @return
     */
    bool isPrint(const std :: string &);
    /**
     * is string save token
     * @return
     */
    bool isSave(const std :: string &);
    /**
     * is string load token
     * @return
     */
    bool isLoad(const std :: string &);
    /**
     * is string + token
     * @return
     */
    bool isPlus(const std :: string &);
    /**
     * is string - token
     * @return
     */
    bool isMinus(const std :: string &);
    /**
     * is string ( token
     * @return
     */
    bool isLeftRoundBracket( const std :: string &);
    /**
     * is string ) token
     * @return
     */
    bool isRightRoundBracket(const std :: string &);
    /**
     * is string = token
     * @return
     */
    bool isAssign(const std :: string &);
    /**
     * is string Print token
     * @return
     */
    bool isPrintTable(const std :: string &);
    /**
     * is string Star token
     * @return
     */
    bool isStar(const std :: string & );
    /**
     * is string Dot token
     * @param state of final state machine
     * @return
     */
    bool isDot(const std :: string & );
    /**
     * is string Slash token
     * @param state of final state machine
     * @return
     */
    bool isSlash(const std :: string & );
    /**
     * is string Variable token
     * @param state of final state machine
     * @return
     */
    bool isVariable(const std :: string & src);


    std :: queue<std :: string> parseInputTokens(const std :: string & src);
    std :: queue<std :: string> m_inputTokens;
    std :: queue<CToken> m_tokens;
};