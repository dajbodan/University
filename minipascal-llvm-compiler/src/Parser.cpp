#include "Parser.hpp"
#include <experimental/iterator>
#include <iostream>
#include <set>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"

Token match(const std::string& rule, Lexer& lex, std::initializer_list<TokenType> tokenTypes)
{
    for (const auto& tokenType : tokenTypes) {
        if (auto token = lex.match(tokenType)) {
            std::ostringstream oss;
            oss << "match " << token.value();


            return token.value();
        }
    }

    throw std :: runtime_error("runtime error\n");
}


    Token match(const std::string& rule, Lexer& lex, TokenType tokenType)
    {
        return match(rule, lex, {tokenType});
    }



    std::shared_ptr<ExprASTNode> Parser :: Expression()
    {

        switch (m_Lexer.peek().type()) {
            case TokenType::INTEGER_LITERAL:
            case TokenType::VARIABLE :
            case TokenType::LPAR:
            case TokenType::MINUS:

                return EqualityExpression();
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::EqualityExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::INTEGER_LITERAL:
            case TokenType::VARIABLE :
            case TokenType::LPAR:
            case TokenType::MINUS: {
                auto lhs = RelationalExpression();
                return EqualityExpressionX(std :: move(lhs));
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::EqualityExpressionX(std::shared_ptr<ExprASTNode> lhs)
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::NEQ:
            case TokenType::EQ: {

                auto typeToken = m_Lexer.peek().type();
                auto op = match("EqualityExpression", m_Lexer, {TokenType::EQ, TokenType::NEQ});
                auto rhs = RelationalExpression();
                auto relOp = std ::make_unique<BinOpASTNode>(typeToken, std :: move(lhs), std :: move(rhs));
                return EqualityExpressionX(std :: move(relOp));
            }
            case TokenType::SEMICOLON:
            case TokenType::RPAR:
            case TokenType::TO :
            case TokenType::DO :
            case TokenType::DOWNTO :
                return lhs;
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::RelationalExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::MINUS:
            case TokenType::INTEGER_LITERAL:
            case TokenType::VARIABLE :
            case TokenType::LPAR: {
                auto lhs = AdditiveExpression();
                return RelationalExpressionX(std :: move(lhs));
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::RelationalExpressionX(std::shared_ptr<ExprASTNode> lhs)
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::LT:
            case TokenType::LE:
            case TokenType::GT:
            case TokenType::GE: {
                auto tokenType = m_Lexer.peek().type();
                auto op = match("RelationalExpression", m_Lexer, {TokenType::LT, TokenType::LE, TokenType::GT, TokenType::GE});
                auto rhs = AdditiveExpression();
                auto addOp = std ::make_unique<BinOpASTNode>(tokenType, std :: move(lhs), std :: move(rhs));
                return RelationalExpressionX(std :: move( addOp));
            }
            case TokenType::NEQ:
            case TokenType::EQ:
            case TokenType::RPAR:
            case TokenType::EOI:
            case TokenType::SEMICOLON:
            case TokenType::TO :
            case TokenType::DOWNTO :
            case TokenType::DO :

                return lhs;
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::AdditiveExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::MINUS:
            case TokenType::INTEGER_LITERAL:
            case TokenType::VARIABLE :
            case TokenType::LPAR: {

                auto lhs = MultiplicativeExpression();
                return AdditiveExpressionX(std :: move(lhs));
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser ::AdditiveExpressionX(std::shared_ptr<ExprASTNode> lhs)
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::PLUS:
            case TokenType::MINUS: {
                auto tokenType = m_Lexer.peek().type();
                auto op = match("AdditiveExpression", m_Lexer, {TokenType::PLUS, TokenType::MINUS});

                auto rhs = MultiplicativeExpression();
                auto addOp = std ::make_unique<BinOpASTNode>(tokenType, std :: move(lhs), std :: move(rhs));
                return AdditiveExpressionX(std :: move(addOp));
            }
            case TokenType::LT:
            case TokenType::LE:
            case TokenType::GT:
            case TokenType::GE:
            case TokenType::NEQ:
            case TokenType::EQ:
            case TokenType::RPAR:
            case TokenType::EOI:
            case TokenType::SEMICOLON :
            case TokenType::TO :
            case TokenType::DOWNTO :
            case TokenType::DO :
                return lhs;
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser :: MultiplicativeExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::MINUS:
            case TokenType::INTEGER_LITERAL:
            case TokenType::VARIABLE :
            case TokenType::LPAR: {
              //  m_Lexer.match(TokenType :: LPAR);

                auto lhs = UnaryExpression();
                return MultiplicativeExpressionX(std :: move(lhs));
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser :: MultiplicativeExpressionX(std::shared_ptr<ExprASTNode> lhs)
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::ASTERISK:
            case TokenType::SLASH:
            case TokenType::PERCENT_SIGN: {
                auto tokenType = m_Lexer.peek().type();
                auto op = match("MultiplicativeExpression", m_Lexer, {TokenType::ASTERISK, TokenType::SLASH, TokenType::PERCENT_SIGN});
                auto rhs = UnaryExpression();
                auto multiOp = std ::make_unique<BinOpASTNode>(tokenType, std :: move(lhs), std :: move(rhs));
                return MultiplicativeExpressionX(std :: move(multiOp));
            }
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::LT:
            case TokenType::LE:
            case TokenType::GT:
            case TokenType::GE:
            case TokenType::NEQ:
            case TokenType::EQ:
            case TokenType::RPAR:
            case TokenType::EOI:
            case TokenType::SEMICOLON:
            case TokenType::TO :
            case TokenType::DOWNTO :
            case TokenType::DO :
                return lhs;
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }




    std::shared_ptr<ExprASTNode> Parser :: UnaryExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::INTEGER_LITERAL:
            case TokenType::LPAR:
            case TokenType::VARIABLE :
                //m_Lexer.match(TokenType :: LPAR);

                return PrimaryExpression();
            case TokenType::MINUS: {
                auto op = match("UnaryExpression", m_Lexer, TokenType::MINUS);
                return std :: make_unique<UnaryOpASTNode>(op.type(), std :: move(UnaryExpression()));
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

    std::shared_ptr<ExprASTNode> Parser :: PrimaryExpression()
    {
        switch (m_Lexer.peek().type()) {
            case TokenType::INTEGER_LITERAL: {
                auto val = m_Lexer.peek().value();
                if(!val.has_value())
                    throw std :: runtime_error("expected integer with value. But there is not value\n");
                auto token = match("PrimaryExpression", m_Lexer, TokenType::INTEGER_LITERAL);
                return std ::make_unique<LiteralASTNode>(val.value());
            }
            case TokenType::LPAR: {
                match("PrimaryExpression", m_Lexer, TokenType::LPAR);
                auto res = Expression();
                match("PrimaryExpression", m_Lexer, TokenType::RPAR);
                return res;
            }
            case TokenType::VARIABLE :
            {
                auto name = m_Lexer.peek().getStrValue().value();
                m_Lexer.match(m_Lexer.peek().type());
                std :: optional<int> index = {};
                if(m_Lexer.peek().type() == TokenType :: SQUAREBRACKETLEFT)
                {
                    int sign = 1;
                    m_Lexer.match(m_Lexer.peek().type());
                    if(m_Lexer.peek().type() == TokenType :: MINUS)
                    {
                        sign = -1;
                        m_Lexer.match(m_Lexer.peek().type());
                    }
                    if(m_Lexer.peek().type() != TokenType :: INTEGER_LITERAL)
                        throw std :: runtime_error("expect integer");
                    index = m_Lexer.peek().value().value() * sign;
                    m_Lexer.match(m_Lexer.peek().type());
                    if(m_Lexer.peek().type() != TokenType :: SQUAREBRACKETRIGHT)
                        throw std :: runtime_error("expect ]");
                    m_Lexer.match(m_Lexer.peek().type());
                }

                return std ::make_unique<DeclRefASTNode>(name, index);
            }
            default:
                throw std :: runtime_error("runtime error\n");
        }
    }

std::shared_ptr<ASTNode> Parser :: Start(std ::shared_ptr<ProgramASTNode> src )
{
    switch(m_Lexer.peek().type())
    {
        case TokenType::CONST:
        {
            m_Lexer.match(m_Lexer.peek().type());
            auto constVar = m_Lexer.peek();
            if(! constVar.getStrValue().has_value())
                throw std :: runtime_error("expected string variable");
            m_Lexer.match(m_Lexer.peek().type());
            m_table.insert({
                                    constVar.getStrValue().value(),
                                    { constVar.getStrValue().value(), nullptr, nullptr, false, false, nullptr}
                                }
                          );
            if(m_Lexer.peek().type() != TokenType :: ASSIGN)
                throw std :: runtime_error("expected  = ");
            m_Lexer.match(m_Lexer.peek().type());
            auto  expr = Expression();
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expected semicolon");
            m_Lexer.match(m_Lexer.peek().type());
            src->insertStatement(
                                    std ::make_unique<VarDeclASTNode>(
                                            constVar.getStrValue().value(),
                                            std ::make_unique<TypeASTNode>(TypeASTNode :: Type ::INT),
                                             std :: move(expr)
                                             )
                            );
            auto tempConsts = constGen();
            for(auto & it : tempConsts)
                src->insertStatement(std :: move(it));
            return Start(std :: move(src));
        }
        case TokenType::BEGIN :
        {
            m_Lexer.match(m_Lexer.peek().type());
            auto temp = statement();
            for(auto & it : temp)
                src->insertStatement(std :: move(it));
            if(m_Lexer.peek().type() != TokenType::END)
                throw std :: runtime_error("expect end");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType ::DOT)
                throw std :: runtime_error("expect dot");
            return src;
        }
        case TokenType::VAR :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: VARIABLE)
                throw std :: runtime_error("expected variable\n");
            auto nameVar = m_Lexer.peek().getStrValue().value();
            m_Lexer.match(m_Lexer.peek().type());
            m_table.insert({
                                   nameVar,
                                   { nameVar, nullptr, nullptr, true, false, nullptr}
                           });
            std :: vector<std :: string> vecNames = {nameVar};
            auto tempVariableGen = variableGen();
            std ::move(tempVariableGen.begin(), tempVariableGen.end(), std ::back_inserter(vecNames));
            auto tempResGen = typeGen(vecNames);
            for(auto & it : tempResGen)
                src->insertStatement(std :: move(it));
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expected semicolon");
            m_Lexer.match(m_Lexer.peek().type());
            return Start(std :: move(src));
        }
        case TokenType::EOI :
            return src;

        default:
            throw std :: runtime_error("runtime error\n");

    }
}

std :: vector<std :: shared_ptr<StatementASTNode>> Parser :: typeGen(const std :: vector<std :: string> & vecNames)
{
    std :: vector<std :: shared_ptr<StatementASTNode>> res;
    switch(m_Lexer.peek().type())
    {
        case TokenType::INTEGER :
        {

            m_Lexer.match(m_Lexer.peek().type());
            for(auto & it : vecNames)
            {
                m_table[it].type = std ::make_shared<TypeASTNode>(TypeASTNode :: Type ::INT);
                res.push_back(std::make_unique<VarDeclASTNode>(
                        it,
                        std::make_unique<TypeASTNode>(TypeASTNode::Type::INT),
                        std::make_unique<LiteralASTNode>(0)));
            }

            break;
        }
        case TokenType:: ARRAY :
        {
            int sign = 1;
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: SQUAREBRACKETLEFT)
                throw std :: runtime_error("expected square bracket left");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() == TokenType :: MINUS)
            {
                sign = -1;
                m_Lexer.match(m_Lexer.peek().type());
            }
            if(m_Lexer.peek().type() != TokenType :: INTEGER_LITERAL)
                throw std :: runtime_error("expected integer literal");

            int begin = m_Lexer.peek().value().value() * sign;
            sign = 1;

            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: DOT)
                throw std :: runtime_error("expected dot");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: DOT)
                throw std :: runtime_error("expected dot");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() ==TokenType :: MINUS )
            {
                sign = 1;
                m_Lexer.match(m_Lexer.peek().type());
            }
            if(m_Lexer.peek().type() != TokenType :: INTEGER_LITERAL)
                throw std :: runtime_error("expected integer literal");
            int end = m_Lexer.peek().value().value() * sign;
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: SQUAREBRACKETRIGHT)
                throw std :: runtime_error("expect square bracket right");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: VARIABLE || m_Lexer.peek().getStrValue().value() != "of")
                throw std :: runtime_error("expect of");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: INTEGER )
                throw std :: runtime_error("expect integer");
            m_Lexer.match(m_Lexer.peek().type());
            for(auto & it : vecNames)
            {
                m_table[it].type = std ::make_shared<TypeASTNode>(TypeASTNode :: Type ::Array, abs(begin) + abs(end));

                auto temp = std ::make_shared<ArrayASTNode>( ArrayASTNode(
                                                                    begin, abs(begin) + abs(end), it
                                                            ));
                m_table[it].m_array = temp;
                res.push_back(std :: move(temp));

            }
            break;
        }
        default:

            throw std :: runtime_error("type is not valid");
    }
    return res;
}

std :: vector<std :: shared_ptr<StatementASTNode>> Parser :: statement()
{
    std :: vector<std :: shared_ptr<StatementASTNode>> res;
    switch (m_Lexer.peek().type())
    {
        case TokenType::WRITELN :
        {
            res.push_back(std ::make_unique<ExpressionStatementASTNode>(std :: move(KeyFunction())));
            auto tempStatement = statement();
            for(auto & it : tempStatement)
                res.push_back(std :: move(it));
            break;
        }
        case TokenType::READLN:
        {
            res.push_back(std ::make_unique<ExpressionStatementASTNode>(std :: move(KeyFunction())));
            auto tempStatement = statement();
            for(auto & it : tempStatement)
                res.push_back(std :: move(it));
            break;
        }
        case TokenType::IF :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType::LPAR)
                throw std :: runtime_error("Expected lpar\n");
            m_Lexer.match(m_Lexer.peek().type());
            auto expr = Expression();                           //COND
            if(expr == nullptr || m_Lexer.peek().type() != TokenType :: RPAR)
                throw std :: runtime_error("expr is nullptr or expected rpar");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: THEN)
                throw std :: runtime_error("expected then");
            m_Lexer.match(m_Lexer.peek().type());
            bool isBeginUsed = false;
            if(m_Lexer.peek().type() == TokenType :: BEGIN)
            {
                isBeginUsed = true;
                m_Lexer.match(m_Lexer.peek().type());
            }
            auto states = statement();                                      //TRUE BRANCH
            if(isBeginUsed &&  m_Lexer.match(TokenType :: END))
            {
                //m_Lexer.match(TokenType :: END);
                m_Lexer.match(TokenType :: SEMICOLON);
            }
            else if(! isBeginUsed )
            {
                m_Lexer.match(TokenType :: END);
            }
            else
            {
                throw std :: runtime_error("expected end");
            }
            std :: vector<std :: shared_ptr<StatementASTNode>> ElseBody;                                                           //FALSE BRANCH
            if(m_Lexer.match(TokenType :: ELSE).has_value())
            {
                if(! m_Lexer.match(TokenType :: THEN).has_value())
                    throw std :: runtime_error("expect THEN");
                auto Else = statement();
                std :: move(Else.begin(), Else.end(), std ::back_inserter(ElseBody));
                if(! m_Lexer.match(TokenType :: END).has_value())
                    throw std :: runtime_error("expect END");
            }

            res.push_back(std ::make_unique<IfASTNode>(std :: move(expr), std :: move(states), std :: move(ElseBody)));
            auto temp = statement();
            for(auto & it : temp)
                res.push_back(std :: move(it));
            break;
        }
        case TokenType::FOR :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: VARIABLE)
                throw std :: runtime_error("expected variable");
            auto var = std ::make_shared<DeclRefASTNode>(m_Lexer.peek().getStrValue().value());
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: DOUBLE_DOT )
                throw std :: runtime_error("Expected double dot");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: ASSIGN)
                throw std :: runtime_error("expected assign");
            m_Lexer.match(m_Lexer.peek().type());

            auto init = Expression();
            ForAstNode :: Type  type = ForAstNode :: Type :: UNKNOWN;
            if(m_Lexer.match(TokenType :: TO))
                type = ForAstNode :: Type :: INC;
            else if(m_Lexer.match(TokenType :: DOWNTO))
                type = ForAstNode :: Type :: DEC;
            else
                throw std :: runtime_error("expected to or downto");
            auto end = Expression();

            if(m_Lexer.peek().type() != TokenType :: DO )
                throw std :: runtime_error("expect do");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: BEGIN)
                throw std :: runtime_error("expect begin");
            m_Lexer.match(m_Lexer.peek().type());
            auto temp = statement();
            auto forNode = std ::make_shared<ForAstNode>(ForAstNode(var,type, init, end, temp ));
            res.push_back(forNode);
            if(m_Lexer.peek().type() != TokenType :: END)
                throw std :: runtime_error("expect end");
            m_Lexer.match(m_Lexer.peek().type());
            m_Lexer.match(TokenType :: SEMICOLON);
            auto tempT = statement();
            for(auto  it : tempT)
                res.push_back(it);
            break;
        }
        case TokenType::WHILE :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: LPAR)
                throw std :: runtime_error("expected (");
            m_Lexer.match(m_Lexer.peek().type());
            auto cond = Expression();
            if(m_Lexer.peek().type() != TokenType :: RPAR)
                throw std :: runtime_error("expected )");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: DO)
                throw std :: runtime_error("expected do");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: BEGIN)
                throw std :: runtime_error("expected begin");
            m_Lexer.match(m_Lexer.peek().type());
            auto body = statement();
            if(m_Lexer.peek().type() != TokenType :: END)
                throw std :: runtime_error("expect end");
            m_Lexer.match(m_Lexer.peek().type());

            res.push_back(std ::make_shared<WhileASTNode>(cond, body));
            auto tempT = statement();
            for(auto  it : tempT)
                res.push_back(it);
            break;
        }
        case TokenType::VARIABLE :
        {
            auto nameVar = m_Lexer.peek().getStrValue().value();
            m_Lexer.match(m_Lexer.peek().type());
            std :: optional<int> index = {};
            if(m_Lexer.peek().type() == TokenType :: SQUAREBRACKETLEFT)
            {
                m_Lexer.match(m_Lexer.peek().type());
                int sign = 1;
                if(m_Lexer.peek().type() == TokenType ::MINUS)
                {
                    sign = -1;
                    m_Lexer.match(m_Lexer.peek().type());
                }
                if(m_Lexer.peek().type() != TokenType::INTEGER_LITERAL)
                    throw std :: runtime_error("expected integer");
                index = m_Lexer.peek().value().value() * sign;
                m_Lexer.match(m_Lexer.peek().type());
                if(m_Lexer.peek().type() != TokenType :: SQUAREBRACKETRIGHT)
                    throw std :: runtime_error("expected ]");
                m_Lexer.match(m_Lexer.peek().type());
            }
            if(m_Lexer.peek().type() != TokenType :: DOUBLE_DOT)
                throw std :: runtime_error("expected double dot");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: ASSIGN)
                throw std :: runtime_error("expected assign");
            m_Lexer.match(m_Lexer.peek().type());
            auto expr = Expression();
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expected semicolon");
            m_Lexer.match(m_Lexer.peek().type());
            res.push_back(std ::make_shared<AssignASTNode>(std ::make_shared<DeclRefASTNode>(nameVar, index), expr));
            auto tempRes = statement();
            for(auto & it : tempRes)
                res.push_back(it);
            break;
        }
        case TokenType::END:
            //m_Lexer.match(m_Lexer.peek().type());
            break;
        default:
            throw std :: runtime_error("not valid statement");
    }
    return res;
}


std :: shared_ptr<FunCallASTNode> Parser :: KeyFunction()
{
    switch (m_Lexer.peek().type())
    {
        case TokenType::WRITELN :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: LPAR)
                throw std :: runtime_error("expect LPAR\n");
            m_Lexer.match(m_Lexer.peek().type());
            std :: vector<std :: shared_ptr<ExprASTNode>> temp;

                auto expr = Expression();
                temp.push_back(std :: move(expr));

            if(m_Lexer.peek().type() != TokenType :: RPAR)
                throw std :: runtime_error("expect RPAR\n");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expect semicolon\n");
            m_Lexer.match(m_Lexer.peek().type());
            return std ::make_shared<FunCallASTNode>(FunCallASTNode("writeln", std :: move(temp)));
        }
        case TokenType::READLN :
        {
            std :: optional<int> index;
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: LPAR)
                throw std :: runtime_error("expect ( ");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: VARIABLE )
                throw std :: runtime_error("readln : expect variable");
            auto name = m_Lexer.peek().getStrValue().value();
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() == TokenType :: SQUAREBRACKETLEFT)
            {
                m_Lexer.match(m_Lexer.peek().type());
                int sign = 1;
                if(m_Lexer.peek().type() == TokenType ::MINUS)
                {
                    sign = -1;
                    m_Lexer.match(m_Lexer.peek().type());
                }
                if(m_Lexer.peek().type() != TokenType :: INTEGER_LITERAL)
                    throw std :: runtime_error("readln : expect integer");
                index = m_Lexer.peek().value().value() * sign;
                m_Lexer.match(m_Lexer.peek().type());
                if(m_Lexer.peek().type() != TokenType :: SQUAREBRACKETRIGHT)
                    throw std :: runtime_error("expect ]");
                m_Lexer.match(m_Lexer.peek().type());

            }
            std :: vector<std :: shared_ptr<ExprASTNode>> temp;
            temp.push_back(std :: move(std ::make_unique<DeclRefASTNode>(name, index)));

            if(m_Lexer.peek().type() != TokenType :: RPAR)
                throw std :: runtime_error("expect ) ");
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expect ; ");
            m_Lexer.match(m_Lexer.peek().type());
            return std ::make_unique<FunCallASTNode>(FunCallASTNode("readln", std :: move(temp)));
        }
        default :
            throw std :: runtime_error("key function is not recognized\n");
    }
}




std :: vector<std :: shared_ptr<VarDeclASTNode>> Parser :: constGen()
{
    std :: vector<std :: shared_ptr<VarDeclASTNode>> res;
    switch (m_Lexer.peek().type())
    {
        case TokenType::VARIABLE :
        {
            if(! m_Lexer.peek().getStrValue().has_value())
                throw std :: runtime_error("runtime error");
            auto name = m_Lexer.peek().getStrValue().value();
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: ASSIGN)
                throw std :: runtime_error("expected  = ");
            m_Lexer.match(m_Lexer.peek().type());
            auto  expr = Expression();
            if(m_Lexer.peek().type() != TokenType :: SEMICOLON)
                throw std :: runtime_error("expected semicolon");
            m_Lexer.match(m_Lexer.peek().type());
            res.push_back(
                            std :: make_unique<VarDeclASTNode>(
                                             name,
                                             std ::make_unique<TypeASTNode>(TypeASTNode :: Type ::INT),
                                             std :: move(expr)
                                             )
                          );
            m_table.insert({
                                   name,
                                   { name, nullptr, nullptr, false, false, nullptr}
                           }
                        );
            auto temp = constGen();
            for(auto & it : temp)
                res.push_back(std :: move(it));
            break;
        }
        case TokenType::CONST :
        case TokenType::BEGIN :
        case TokenType::VAR :
            break;
        default :
            std :: runtime_error("runtime error\n");
    }
    return res;
}


std :: vector<std :: string> Parser :: variableGen()
{
    std :: vector<std :: string> res;
    switch(m_Lexer.peek().type())
    {
        case TokenType::COMMA :
        {
            m_Lexer.match(m_Lexer.peek().type());
            if(m_Lexer.peek().type() != TokenType :: VARIABLE)
                throw std :: runtime_error("expected variable\n");
            auto nameVar = m_Lexer.peek().getStrValue().value();
            m_Lexer.match(m_Lexer.peek().type());
            auto temp = variableGen();
            res.push_back(
                    nameVar
            );
            m_table.insert({
                                   nameVar,
                                   { nameVar, nullptr, nullptr, true, false, nullptr}
                           });

            for(auto & it : temp)
                res.push_back(std :: move(it));
            break;
        }
        case TokenType::DOUBLE_DOT :
            m_Lexer.match(TokenType :: DOUBLE_DOT);
            break;
        default:
            throw std :: runtime_error("runtime error\n");
    }


    return res;
}




