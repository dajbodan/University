#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <ostream>
#include "ast.hpp"


GenContext::GenContext(const std::string& moduleName)
        : builder(ctx)
        , module(moduleName, ctx)
{
}


llvm::Value* TypeASTNode::codegen(GenContext& gen) const
{

    return nullptr;
}

llvm::Type* TypeASTNode::genType(GenContext& gen) const
{
    switch (m_type) {
        case Type::DOUBLE:
            return llvm::Type::getDoubleTy(gen.ctx);
        case Type::INT:
            return llvm::Type::getInt32Ty(gen.ctx);
        case Type::Array :
        {
            if(! m_elements.has_value())
                throw std :: runtime_error("array does not know how many elements there is");
            return llvm::ArrayType::get(gen.builder.getInt32Ty(), m_elements.value());
        }
        default:
            assert(false);
    }
}

llvm::Value* BinOpASTNode::codegen(GenContext& gen) const
{
    auto lhs = m_lhs->codegen(gen);
    auto rhs = m_rhs->codegen(gen);
    if(rhs->getType()->isPointerTy())
        rhs = gen.builder.CreateLoad( llvm::Type::getInt32Ty(gen.ctx), rhs);
    if(lhs->getType()->isPointerTy())
        lhs = gen.builder.CreateLoad( llvm::Type::getInt32Ty(gen.ctx), lhs);

    assert(lhs);
    assert(rhs);

    bool dblArith = lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy();
    auto maybeSIToFP = [&gen](llvm::Value* val) {
        if (!val->getType()->isDoubleTy())
            return gen.builder.CreateSIToFP(val, llvm::Type::getDoubleTy(gen.ctx));
        return val;
    };

    switch (m_op) {
        case TokenType::PLUS:
            if (dblArith)
                return gen.builder.CreateFAdd(maybeSIToFP(lhs), maybeSIToFP(rhs), "add");
            else
                return gen.builder.CreateAdd(lhs, rhs, "add");
        case TokenType::MINUS :
            return gen.builder.CreateSub(lhs, rhs, "sub");
        case TokenType::SLASH:
            if (dblArith)
                return gen.builder.CreateFDiv(maybeSIToFP(lhs), maybeSIToFP(rhs), "div");
            else
                return gen.builder.CreateSDiv(lhs, rhs, "div");
        case TokenType::EQ:
            if (dblArith)
                return gen.builder.CreateFCmpOEQ(maybeSIToFP(lhs), maybeSIToFP(rhs), "eq");
            else
                return gen.builder.CreateICmpEQ(lhs, rhs, "eq");
        case TokenType::NEQ :
            return gen.builder.CreateICmpNE(lhs, rhs, "neq");
        case TokenType::ASTERISK:
                return gen.builder.CreateMul(lhs, rhs, "mull");
        case TokenType::LT :
                return gen.builder.CreateICmpSLT(lhs, rhs, "less than");
        case TokenType::GT :
                return gen.builder.CreateICmpSGT(lhs, rhs, "signed greater than");
        case TokenType::LE :
                return gen.builder.CreateICmpSLE(lhs, rhs, "le");
        default:
            throw "unimplemented";
    }
}

llvm::Value* UnaryOpASTNode::codegen(GenContext& gen) const
{
    auto expr = m_expr->codegen(gen);

    assert(expr);

    switch (m_op) {
        case TokenType::MINUS:
            if (expr->getType()->isDoubleTy())
                return gen.builder.CreateFSub(
                        llvm::ConstantFP::get(llvm::Type::getDoubleTy(gen.ctx), llvm::APFloat(0.0)),
                        expr,
                        "unminus");
            return gen.builder.CreateSub(
                    llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), 0),
                    expr,
                    "unminus");
        default:
            throw "unimplemented";
    }
}

llvm::Value* AssignASTNode::codegen(GenContext& gen) const
{
    //auto* store = m_var->getStore(gen);
    auto* store = m_var->codegen(gen);
    auto expr = m_expr->codegen(gen);

    //if (store->getAllocatedType()->isDoubleTy())
     //   expr = gen.builder.CreateSIToFP(expr, llvm::Type::getDoubleTy(gen.ctx));

    gen.builder.CreateStore(expr, store);
    return nullptr;
}

llvm::Value* LiteralASTNode::codegen(GenContext& gen) const
{
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), m_value);
}

llvm::Value* DeclRefASTNode::   codegen(GenContext& gen) const
{
    assert(gen.symbolTable.contains(m_var)
                || gen.symbolTable.find(m_var)->second.isVarOrArray
                || (! gen.symbolTable.find(m_var)->second.isVarOrArray && ! gen.symbolTable.find(m_var)->second.isLoaded ));
    auto & symbol = gen.symbolTable[m_var];
    symbol.isLoaded = true;
    if(m_index.has_value())
    {
        if(symbol.m_array == nullptr)
            throw std :: runtime_error("left side is not array");
        llvm::Value * ind [] = {gen.builder.getInt64(0), gen.builder.getInt64(m_index.value() + abs(symbol.m_array->getOffset()))};
        return gen.builder.CreateGEP(symbol.type->genType(gen), symbol.store, ind, "elemPtr");
    }
    else
    {

        return symbol.store;
    }
}

llvm::AllocaInst* DeclRefASTNode::getStore(GenContext& gen) const
{

    assert(gen.symbolTable.contains(m_var));
    return gen.symbolTable[m_var].store;
}

llvm::Value* FunCallASTNode::codegen(GenContext& gen) const
{
    auto* func = gen.module.getFunction(m_func);
    assert(func);

    std::vector<llvm::Value*> args;
    for (const auto& arg : m_args)
    {
        auto temp = arg->codegen(gen);
        if(temp->getType()->isPointerTy() && m_func != "readln")
        {
            temp = gen.builder.CreateLoad(llvm::Type::getInt32Ty(gen.ctx), temp);
        }
        args.emplace_back(temp);
    }

    return gen.builder.CreateCall(func, args);
}

llvm::Value* IfASTNode::codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock * BBElse = llvm::BasicBlock::Create(gen.ctx, "else", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);

    auto cond = m_cond->codegen(gen);
    gen.builder.CreateCondBr(cond, BBbody, BBElse);

    gen.builder.SetInsertPoint(BBbody);
    for(auto & it : m_body)
        it->codegen(gen);
    gen.builder.CreateBr(BBafter);

    gen.builder.SetInsertPoint(BBElse);
    for(auto & it : m_else)
        it->codegen(gen);
    gen.builder.CreateBr(BBafter);

    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* VarDeclASTNode::codegen(GenContext& gen) const
{
    assert(!gen.symbolTable.find(m_var)->second.store);

    auto* store = gen.builder.CreateAlloca(m_type->genType(gen), 0, m_var);
    //gen.symbolTable[m_var] = {m_var, m_type.get(), true, store};
    auto & getSymbol = gen.symbolTable[m_var];
    getSymbol.isLoaded = true;
    getSymbol.store = store;
    getSymbol.type = m_type;
    if (m_expr) {
        auto expr = m_expr->codegen(gen);
        assert(expr);
        if (store->getAllocatedType()->isDoubleTy())
            expr = gen.builder.CreateSIToFP(expr, llvm::Type::getDoubleTy(gen.ctx));
        //if(getSymbol.type->getType() == TypeASTNode ::Type ::Array)

        gen.builder.CreateStore(expr, store);
    }

    return nullptr;
}

llvm::Value* ArrayASTNode :: codegen(GenContext& gen) const
{
    assert(!gen.symbolTable.find(m_name)->second.store || gen.symbolTable.find(m_name) != gen.symbolTable.end());
    auto & getSymbol = gen.symbolTable[m_name];
    getSymbol.store = gen.builder.CreateAlloca( getSymbol.type->genType(gen), nullptr, m_name);

    return nullptr;
}

llvm::Value* ExpressionStatementASTNode::codegen(GenContext& gen) const
{
    return m_expr->codegen(gen);
}


llvm::Value* WhileASTNode :: codegen(GenContext& gen) const
{
    auto parent = gen.builder.GetInsertBlock()->getParent();
    llvm::BasicBlock * cond = llvm::BasicBlock::Create(gen.ctx, "cond", parent);
    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);
    gen.builder.CreateCondBr(llvm :: ConstantInt :: get(llvm::Type::getInt1Ty(gen.ctx), 1), cond, BBafter);

    gen.builder.SetInsertPoint(cond);
    auto tempCond = m_cond->codegen(gen);
    gen.builder.CreateCondBr(tempCond, BBbody, BBafter);

    gen.builder.SetInsertPoint(BBbody);
    for(auto it : m_body)
    {
        it->codegen(gen);
    }
    gen.builder.CreateCondBr(llvm :: ConstantInt :: get(llvm::Type::getInt1Ty(gen.ctx), 1), cond, BBafter);

    gen.builder.SetInsertPoint(BBafter);

}

llvm::Value* ForAstNode :: codegen(GenContext& gen) const
{

    auto parent = gen.builder.GetInsertBlock()->getParent();
    std ::make_unique<AssignASTNode>(m_var, std :: move(m_init))->codegen(gen);

    llvm::BasicBlock * cond = llvm::BasicBlock::Create(gen.ctx, "cond", parent);
    llvm::BasicBlock* BBbody = llvm::BasicBlock::Create(gen.ctx, "body", parent);
    llvm::BasicBlock* BBafter = llvm::BasicBlock::Create(gen.ctx, "after", parent);
    gen.builder.CreateCondBr(llvm :: ConstantInt :: get(llvm::Type::getInt1Ty(gen.ctx), 1), cond, BBafter);
    gen.builder.SetInsertPoint(cond);
    llvm::Value * tempCond = nullptr;
    if(m_type == Type :: INC)
        tempCond = std ::make_shared<BinOpASTNode>(TokenType :: LT , m_var, m_End)->codegen(gen);
    else if(m_type == Type :: DEC)
        tempCond = std ::make_shared<BinOpASTNode>(TokenType :: GT , m_var, m_End)->codegen(gen);

    gen.builder.CreateCondBr(tempCond, BBbody, BBafter);

    gen.builder.SetInsertPoint(BBbody);
    for(auto it : m_body)
        it->codegen(gen);
    std :: shared_ptr<BinOpASTNode> it = nullptr;
    if(m_type == Type ::INC)
        it = std ::make_shared<BinOpASTNode>(TokenType :: PLUS, m_var, std ::make_shared<LiteralASTNode>(1));
    else if(m_type == Type :: DEC)
        it = std ::make_shared<BinOpASTNode>(TokenType :: MINUS, m_var, std ::make_shared<LiteralASTNode>(1));
    else
        throw std :: runtime_error("unknown oper in for");
    gen.builder.CreateStore(it->codegen(gen), m_var->codegen(gen));
    if(m_type == Type :: INC)
        gen.builder.CreateCondBr(std ::make_shared<BinOpASTNode>(TokenType :: LT , it, m_End)->codegen(gen), cond, BBafter);
    else if(m_type == Type :: DEC )
        gen.builder.CreateCondBr(std ::make_shared<BinOpASTNode>(TokenType :: GT , it, m_End)->codegen(gen), cond, BBafter);
    gen.builder.SetInsertPoint(BBafter);

    return nullptr;
}

llvm::Value* ProgramASTNode::codegen(GenContext& gen) const
{
    llvm::FunctionType* ftMain = llvm::FunctionType::get(llvm::Type::getInt32Ty(gen.ctx), false);
    llvm::Function* fMain = llvm::Function::Create(ftMain, llvm::Function::ExternalLinkage, "main", gen.module);

    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
                llvm::Type::getVoidTy(gen.ctx),
                {llvm::Type::getInt32Ty(gen.ctx)},
                false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "printI", gen.module);
    }
    {
        llvm::FunctionType* ft = llvm::FunctionType::get(
                llvm::Type::getVoidTy(gen.ctx),
                {llvm::Type::getDoubleTy(gen.ctx)},
                false);
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "printD", gen.module);
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(gen.ctx, "entry", fMain);
    gen.builder.SetInsertPoint(BB);

    for (const auto& s : m_statements) {
        s->codegen(gen);
    }

    gen.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(gen.ctx), 42));
    gen.module.print(llvm::outs(), nullptr);
    return nullptr;
}
