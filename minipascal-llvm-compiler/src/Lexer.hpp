#pragma once
#include <istream>
#include <optional>
#include <unordered_map>
#include <iostream>

enum class TokenType {
    INTEGER_LITERAL,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    PERCENT_SIGN,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NEQ,
    LPAR,
    RPAR,
    EOI,
    AND,
    DOLLAR,
    COMMA,
    VAR,
    INTEGER,
    DOT,
    CONST,
    BEGIN,
    END,
    VARIABLE,
    PROGRAM,
    SEMICOLON,
    ASSIGN,
    WRITELN,
    READLN,
    IF,
    THEN,
    DOUBLE_DOT,
    FOR,
    TO,
    DOWNTO,
    DO,
    ARRAY,
    SQUAREBRACKETLEFT,
    SQUAREBRACKETRIGHT,
    WHILE,
    ELSE

};

class Position {
    unsigned m_line;
    unsigned m_col;

public:
    Position();
    Position(const Position& other);

    friend std::ostream& operator<<(std::ostream& os, const Position& tk) noexcept;
    void advance();
    void line();
};

class Token {
    TokenType m_tokenType;
    std::optional<uint64_t> m_intValue;
    std :: optional<std :: string> m_strValue;
    Position m_position;

public:
    Token(TokenType type, const Position& position, const std::optional<int>& intValue);
    Token(TokenType type, const Position& position, const std::optional<std :: string>& intValue);
    TokenType type() const;
    std::optional<uint64_t> value() const;
    std::optional<std :: string> getStrValue() const;
    const Position& position() const;
};

class Lexer {
    std::istream & m_is;
    std :: unordered_map<std :: string, TokenType> m_keyWords;

    Token m_nextToken;
    Position m_position;
    //void next();
    Token readNextToken();
    void writeRest()
    {
        std :: cerr << "rest : " << std :: endl;
        while(m_is)
        {
            std :: cerr << (char)m_is.get();
        }
        std :: cerr << std :: endl;
    }

public:
    Lexer(std::istream & is);
    std::optional<Token> match(TokenType tokenType);
    Token peek() const;
};

std::ostream& operator<<(std::ostream & os, TokenType tokenType) noexcept;
std::ostream& operator<<(std::ostream & os, const Token& tk) noexcept;
