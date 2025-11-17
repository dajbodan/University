#include <iostream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "ast.hpp"

std :: string readStartProgram(Lexer & lex)
{
    if( ! lex.match(TokenType :: PROGRAM).has_value())
        throw std :: runtime_error("expect program as first token");
    auto temp = lex.match(TokenType :: VARIABLE);
    if(! temp.has_value() || ! temp->getStrValue().has_value())
        throw std :: runtime_error("expect a valid string as second token");
    std :: string res = temp->getStrValue().value();
    if(! lex.match(TokenType :: SEMICOLON).has_value())
        throw std :: runtime_error("expect semicolon  as third token");
    return res;
}

void fileHandler(std :: ifstream & is, char * file)
{
    is.open(file, std :: ios_base::in );
    if(!is)
        throw std ::invalid_argument("can not open file");
}

int main(int argc, char * argv[]) {
    if(argc != 2)
    {
        perror("expected one file on input\n");
        return 1;
    }
    std :: ifstream ifs;

    fileHandler(ifs, argv[1]);
   // fileHandler(ifs, "/Users/dajbodan/Documents/programovani/pjp/FinalFinalOne/testInputSecond.txt");
    Lexer lex(ifs);
    std :: string name = readStartProgram(lex);
    GenContext context(name);

    context.module.getOrInsertFunction("writeln", llvm::FunctionType::get(llvm::Type :: getVoidTy(context.ctx), true));
    context.module.getOrInsertFunction("readln", llvm::FunctionType::get(llvm::Type :: getVoidTy(context.ctx), true));
    Parser parser(lex, context);
    parser.Generate()->codegen(context);



    return 0;
}
