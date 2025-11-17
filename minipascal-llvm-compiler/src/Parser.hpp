#ifndef PJPPROJECT_PARSER_HPP
#define PJPPROJECT_PARSER_HPP

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include <iostream>
#include <fstream>
#include "Lexer.hpp"
#include "ast.hpp"

class Table;

class Parser {
public:
    Parser(Lexer & ifs, GenContext & cont ) : m_Lexer(ifs)
    , MilaContext( cont.ctx)
    , CurTok(0)
    , MilaBuilder(cont.builder)
    , MilaModule(cont.module)
    , m_table(cont.symbolTable)
    { }
    ~Parser() = default;

    bool Parse();                    // parse
    std ::shared_ptr<ASTNode> Generate()
    {
        std ::shared_ptr<ProgramASTNode> program = std ::make_shared<ProgramASTNode>(std::vector<std::shared_ptr<StatementASTNode>>());
        auto res = Start(std :: move(program));
        return res;
    }

private:


    Lexer m_Lexer;                   // lexer is used to read tokens
    int CurTok;                      // to keep the current token

    llvm::LLVMContext & MilaContext;   // llvm context
    llvm::IRBuilder<> & MilaBuilder;   // llvm builder
    llvm::Module & MilaModule;         // llvm module

    std :: map<std :: string, Symbol> & m_table;

    std::shared_ptr<ExprASTNode> Expression();
    std::shared_ptr<ExprASTNode> EqualityExpression();
    std::shared_ptr<ExprASTNode> EqualityExpressionX(std::shared_ptr<ExprASTNode> lhs);
    std::shared_ptr<ExprASTNode> RelationalExpression();
    std::shared_ptr<ExprASTNode> RelationalExpressionX(std::shared_ptr<ExprASTNode> lhs);
    std::shared_ptr<ExprASTNode> AdditiveExpression();
    std::shared_ptr<ExprASTNode> AdditiveExpressionX(std::shared_ptr<ExprASTNode> lhs);
    std::shared_ptr<ExprASTNode> MultiplicativeExpression();

    std::shared_ptr<ExprASTNode> MultiplicativeExpressionX(std::shared_ptr<ExprASTNode> lhs);

    std::shared_ptr<ExprASTNode> UnaryExpression();

    std::shared_ptr<ExprASTNode> PrimaryExpression();

    std :: vector<std :: shared_ptr<StatementASTNode>> typeGen(const std :: vector<std :: string> & src);

    std::shared_ptr<ASTNode> Start(std ::shared_ptr<ProgramASTNode> );

    std :: vector<std :: shared_ptr<VarDeclASTNode>> constGen();
    std :: vector<std :: string> variableGen();
    std :: vector<std ::shared_ptr<VarDeclASTNode>> variableGenSecond();
    std :: vector<std :: shared_ptr<StatementASTNode>> statement();
    std :: shared_ptr<FunCallASTNode> KeyFunction();

};





#endif //PJPPROJECT_PARSER_HPP
