#include "Lexer.hpp"

Position::Position()
        : m_line(1)
        , m_col(1)
{
}

Position::Position(const Position& other) = default;

void Position::advance()
{
    m_col++;
}

void Position::line()
{
    m_line++;
    m_col = 0;
}

Token::Token(TokenType tokenType, const Position& position, const std::optional<int>& intValue = std::nullopt)
        : m_tokenType(tokenType)
        , m_intValue(intValue)
        , m_strValue({})
        , m_position(position)
{
}

Token::Token(TokenType tokenType, const Position& position, const std::optional<std :: string>& strValue )
        : m_tokenType(tokenType)
        , m_intValue({})
        , m_strValue(strValue)
        , m_position(position)
{
}

const Position& Token::position() const
{
    return m_position;
}

TokenType Token::type() const
{
    return m_tokenType;
}

std::optional<uint64_t> Token::value() const
{
    return m_intValue;
}

std::optional<std :: string> Token :: getStrValue() const
{
    return m_strValue;
}

Lexer::Lexer(std::istream& is)
        : m_is(is), m_keyWords({
                                       {"var", TokenType::VAR},
                                       {"const", TokenType :: CONST },
                                       {"begin", TokenType :: BEGIN},
                                       {"end", TokenType :: END},
                                       {"program", TokenType :: PROGRAM},
                                       {"writeln", TokenType :: WRITELN},
                                       {"readln", TokenType :: READLN},
                                       {"if", TokenType :: IF},
                                       {"then", TokenType :: THEN},
                                       {"integer", TokenType :: INTEGER},
                                       {"for", TokenType :: FOR},
                                       {"to", TokenType :: TO},
                                       {"DOWNTO", TokenType :: DOWNTO},
                                       {"do", TokenType :: DO},
                                       {"array", TokenType :: ARRAY},
                                       {"while", TokenType :: WHILE},
                                       {"else", TokenType :: ELSE}
                               })
        , m_nextToken(readNextToken())
{

}

Token Lexer::peek() const
{
    return m_nextToken;
}

std::optional<Token> Lexer::match(TokenType tokenType)
{
    if (peek().type() == tokenType) {
        auto tk = m_nextToken;
        m_nextToken = readNextToken();
        return tk;
    }

    return std::nullopt;
}

Token Lexer::readNextToken()
{
    int intValue = 0;
    std :: string str = "";

    qStart:
    if (m_is.peek() == EOF) {
        return Token(TokenType::EOI, m_position);
    }
    if (std::isspace(m_is.peek())) {

        if (m_is.peek() == '\n') {
            m_position.line();
        } else {
            m_position.advance();
        }
        m_is.get();

        goto qStart;
    }

    switch (m_is.peek()) {
        case '+':
            m_is.get();
            m_position.advance();
            return Token(TokenType::PLUS, m_position);
        case '-':
            m_is.get();
            m_position.advance();
            return Token(TokenType::MINUS, m_position);
        case '*':
            m_is.get();
            m_position.advance();
            goto qAsterisk;
        case '/':
            m_is.get();
            m_position.advance();
            return Token(TokenType::SLASH, m_position);
        case '%':
            m_is.get();
            m_position.advance();
            return Token(TokenType::PERCENT_SIGN, m_position);
        case '<':
            m_is.get();
            m_position.advance();
            goto qLess;
        case '>':
            m_is.get();
            m_position.advance();
            goto qGreater;
        case '=':
            m_is.get();
            m_position.advance();
            goto qEqual;
        case '!':
            m_is.get();
            m_position.advance();
            goto qExclamation;
        case '(':
            m_is.get();
            m_position.advance();
            return Token(TokenType::LPAR, m_position);
        case ')':
            m_is.get();
            m_position.advance();
            return Token(TokenType::RPAR, m_position);
        case '&':
            m_is.get();
            m_position.advance();
            goto qOctal;
        case '$':
            m_is.get();
            m_position.advance();
            goto qHex;
        case '.':
            m_is.get();
            m_position.advance();
            return Token(TokenType :: DOT, m_position);
        case ';':
            m_is.get();
            m_position.advance();
            return Token(TokenType::SEMICOLON, m_position);
        case ':':
            m_is.get();
            m_position.advance();
            return Token(TokenType::DOUBLE_DOT, m_position);
        case '[' :
            m_is.get();
            m_position.advance();
            return Token(TokenType :: SQUAREBRACKETLEFT, m_position);
        case ']' :
            m_is.get();
            m_position.advance();
            return Token(TokenType :: SQUAREBRACKETRIGHT, m_position);
        default:;
           // throw std :: exception();
    }

    if (std::isdigit(m_is.peek())) {
        intValue = m_is.get() - '0';
        m_position.advance();
        goto qInt;
    }

    if(std ::isalpha(m_is.peek()))
    {
        //str = m_is.peek();
        //m_position.advance();
        goto qVar;
    }
    writeRest();
    throw std::runtime_error("Unable to lex next token.");

    qAsterisk:
    switch (m_is.peek()) {
        default:
            return Token(TokenType::ASTERISK, m_position);
    }

    qLess:
    switch (m_is.peek()) {
        case '=':
            m_is.get();
            m_position.advance();
            return Token(TokenType::LE, m_position);
        default:
            return Token(TokenType::LT, m_position);
    }

    qGreater:
    switch (m_is.peek()) {
        case '=':
            m_is.get();
            m_position.advance();
            return Token(TokenType::GE, m_position);
        default:
            return Token(TokenType::GT, m_position);
    }

    qEqual:
    if(std ::isspace(m_is.peek()))
    {
        return Token(TokenType::ASSIGN, m_position);
    }
    switch (m_is.peek()) {
        case '=':
            m_is.get();
            m_position.advance();
            return Token(TokenType::EQ, m_position);
        default:
            writeRest();
            throw std::runtime_error("Unable to lex next token 2.");
    }

    qExclamation:
    switch (m_is.peek()) {
        case '=':
            m_is.get();
            m_position.advance();
            return Token(TokenType::NEQ, m_position);
        default:
            writeRest();
            throw std::runtime_error("Unable to lex next token 3.");
    }

    qInt:
    if (std::isdigit(m_is.peek())) {
        intValue = intValue * 10 + m_is.get() - '0';
        m_position.advance();
        goto qInt;
    } else {
        return Token(TokenType::INTEGER_LITERAL, m_position, intValue);
    }

    qVar :
    if(std ::isalpha(m_is.peek()))
    {
        str += m_is.get();
        m_position.advance();
        if( auto tempRes = m_keyWords.find(str); tempRes != m_keyWords.end())
        {
          //  std :: cout << "here\n";
            return Token(tempRes->second, m_position, str);
        }
        goto qVar;
    }
    else
    {
        return Token(TokenType::VARIABLE, m_position, str);
    }
    qHex :
    if(std ::isspace(m_is.peek()))
    {
        return Token(TokenType :: INTEGER_LITERAL, m_position, intValue);
    }
    switch (tolower(m_is.peek()))
    {
        case '0' :
        case '1' :
        case '2' :
        case '3' :
        case '4' :
        case '5' :
        case '6' :
        case '7' :
        case '8' :
        case '9' :
            intValue = intValue * 16 + m_is.get() - '0';
            m_position.advance();
            goto qHex;
        case 'a' :
            intValue = intValue * 16 + 10;
            m_is.get();
            m_position.advance();
            goto qHex;
        case 'b' :
            intValue = intValue * 16 + 11;
            m_is.get();
            m_position.advance();
            goto qHex;
        case 'c' :
             intValue = intValue * 16 + 12;
             m_is.get();
             m_position.advance();
             goto qHex;
        case 'd' :
            intValue = intValue * 16 + 13;
            m_is.get();
            m_position.advance();
            goto qHex;
        case 'e' :
            intValue = intValue * 16 + 14;
            m_is.get();
            m_position.advance();
            goto qHex;
        case 'f' :
            intValue = intValue * 16 + 15;
            m_is.get();
            m_position.advance();
        default:;
        return Token(TokenType :: INTEGER_LITERAL, m_position, intValue);
    }

    qOctal :
        if(std ::isspace(m_is.peek()))
        {
            return Token(TokenType :: INTEGER_LITERAL, m_position ,intValue );
        }
        switch (tolower(m_is.peek()))
        {
            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
                intValue = intValue * 8 + m_is.get() - '0';
                m_position.advance();
                goto qOctal;
            default:
                return Token(TokenType :: INTEGER_LITERAL, m_position, intValue);

        }
    
    
}

std::ostream& operator<<(std::ostream& os, TokenType tokenType) noexcept
{
    switch (tokenType) {
        case TokenType::INTEGER_LITERAL:
            return os << "INTEGER_LITERAL";
        case TokenType::PLUS:
            return os << "PLUS";
        case TokenType::MINUS:
            return os << "MINUS";
        case TokenType::ASTERISK:
            return os << "ASTERISK";
        case TokenType::SLASH:
            return os << "SLASH";
        case TokenType::PERCENT_SIGN:
            return os << "PERCENT_SIGN";
        case TokenType::LT:
            return os << "LT";
        case TokenType::LE:
            return os << "LE";
        case TokenType::GT:
            return os << "GT";
        case TokenType::GE:
            return os << "GE";
        case TokenType::EQ:
            return os << "EQ";
        case TokenType::NEQ:
            return os << "NEQ";
        case TokenType::LPAR:
            return os << "LPAR";
        case TokenType::RPAR:
            return os << "RPAR";
        case TokenType::EOI:
            return os << "EOI";
        case TokenType::PROGRAM:
            return os << "PROGRAM";
        case TokenType::VARIABLE:
            return os << "VARIABLE";
        case TokenType::ASSIGN :
            return os << "ASSIGN";
        case TokenType::CONST :
            return os << "CONST";
        case TokenType::SEMICOLON :
            return os << "SEMICOLON";
        default:
            return os << "*unknown*" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) noexcept
{
    os << "(Token " << token.type();
    if (token.value()) {
        os << " " << *token.value();
    }
    return os << ")";
}

std::ostream& operator<<(std::ostream& os, const Position& position) noexcept
{
    return os << position.m_line << ':' << position.m_col;
}


