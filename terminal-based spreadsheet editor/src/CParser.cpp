#include "CParser.hpp"

CParser :: CParser(
                    CLexan & lex,
                    std :: shared_ptr<Table> table,
                    std :: function<void(const std :: string &)> Fileloader )
{
    m_table = table;
    if(Fileloader != nullptr)
        m_loader = Fileloader;
    m_root = S(lex);
    if(m_root == nullptr)
        throw std :: exception();
    if(! lex.isEmpty())
        throw std :: exception();
}


bool CParser :: makeAst(AbstractSyntaxTree & outTree)
{

    std :: shared_ptr<ASTAbstractNode> shod = nullptr;
    std :: shared_ptr<ASTAbstractNode> dhod = nullptr;
    wrapperSyntaxTree(m_root, shod, dhod);
    outTree.m_root = shod;
    return true;
}


std :: shared_ptr<CParser :: Node > CParser :: S ( CLexan & lex)
{
    std :: shared_ptr<CParser :: Node> node = std ::make_shared<Node>(Node());
    if(lex.isEmpty())
        return nullptr;
    auto getToken = lex.getNextToken();

    if(getToken.isEqual("print"))
    {
        makeUnaryOperation(node , lex, Rules :: SEVENTEEN, std ::make_shared<PrintFunction>(PrintFunction()));
    }
    else if(getToken.isEqual("PrintTable"))
    {
        lex.popToken();
        if(! (! lex.isEmpty() && lex.getNextToken().isEqual("(")))
            throw std :: exception();
        lex.popToken();
        if(! (! lex.isEmpty() && lex.getNextToken().isEqual(")")))
            throw std :: exception();
        lex.popToken();
        node->m_operUnary = std ::make_shared<PrintTableFunction>(PrintTableFunction(m_table));
        node->m_rule = Rules :: TWENTY;

    }
    else if(getToken.isTypeVariable(CToken :: VARIABLE))
    {
        lex.popToken();
        node->m_rule = Rules :: TWO;
        if(getToken.getTypeValue() == CToken :: STRING)
            node->m_childs.push_back(std :: make_shared<Node>(Node(std :: make_shared<CString>(getToken.getStrNameToken()))));
        else
            throw std :: exception();
        node->m_childs.push_back(S_1(lex));
    }
    else if(getToken.isEqual("load"))
    {
        lex.popToken();
        if(lex.getNextToken().getTypeValue() != CToken :: STRING || ! m_loader.has_value())
            throw std :: exception();
        node->m_operUnary = std ::make_shared<LoadFunction>(LoadFunction( m_loader.value()));
        node->m_type = std :: make_shared<CString>(lex.getNextToken().getStrNameToken());
        node->m_rule = Rules :: EIGHTEEN;
        lex.popToken();

    }
    else if(getToken.isEqual("save"))
    {
        lex.popToken();
        if(lex.getNextToken().getTypeValue() != CToken :: STRING || ! m_loader.has_value())
            throw std :: exception();
        node->m_operUnary = std ::make_shared<SaveFileFunction>(SaveFileFunction( m_table));
        node->m_type = std :: make_shared<CString>(lex.getNextToken().getStrNameToken());
        node->m_rule = Rules :: NINETEEN;
        lex.popToken();
    }
    else
    {
        throw std :: exception();
    }
    return node;
}

std::shared_ptr<CParser :: Node> CParser ::S_1(CLexan &lex)
{
    std :: shared_ptr<CParser :: Node> node = std ::make_shared<Node>(Node()) ;
    if(lex.isEmpty())
        return nullptr;
    auto getToken = lex.getNextToken();
    if(getToken.isEqual("="))
    {
        lex.popToken();
        node->m_rule = Rules :: FOUR;
        node->m_operBinary = std ::make_shared<AssignOperator>(AssignOperator(m_table));
        node->m_childs.push_back(A(lex));
    }
    else
    {
        throw std :: exception();
    }
    return node;
}

std::shared_ptr< CParser :: Node> CParser ::A(CLexan &lex)
{
    std :: shared_ptr<CParser :: Node> node = std ::make_shared<Node>(Node()) ;
    if(lex.isEmpty())
        return nullptr;
    auto getToken = lex.getNextToken();
    if(getToken.isEqual("cos") or
        getToken.isEqual("sin") or
        getToken.isEqual("abs") or
        getToken.isTypeVariable(CToken :: VARIABLE) or getToken.isEqual("("))
    {
        node->m_rule = Rules :: FIVE;
        node->m_childs.push_back(E(lex));
        node->m_childs.push_back(A_1(lex));
    }
    else
    {
        throw std :: exception();
    }
    return node;
}

std::shared_ptr<CParser :: Node> CParser ::A_1(CLexan &lex)
{
    std :: shared_ptr<CParser :: Node> node = std ::make_shared<Node>(Node()) ;
    if(lex.isEmpty() or lex.getNextToken().isEqual(")"))
        node->m_rule = Rules :: NINE;
    else if( lex.getNextToken().isEqual("+"))
        makeBinaryOperation(node, lex, Rules :: SIX, std ::make_shared<PlusFunction>(PlusFunction()));
    else if( lex.getNextToken().isEqual("-"))
        makeBinaryOperation(node, lex, Rules ::TEN, std ::make_shared<MinusFunction>(MinusFunction()));
    else if( lex.getNextToken().isEqual("*"))
        makeBinaryOperation(node, lex, Rules :: ELEVEN, std ::make_shared<MultiplyFunction>(MultiplyFunction()));
    else if( lex.getNextToken().isEqual("/"))
        makeBinaryOperation(node, lex, Rules :: TWELVE, std ::make_shared<DivideFunction>(DivideFunction()));
    else if(lex.isEmpty() or lex.getNextToken().isEqual("."))
        makeBinaryOperation(node, lex, Rules :: THIRTEEN, std ::make_shared<Concat>(Concat()));
    else
        throw std :: exception();
    return node;
}

std::shared_ptr<CParser :: Node> CParser ::E(CLexan &lex)
{
    std :: shared_ptr<CParser :: Node> node = std ::make_shared<Node>(Node());
    if(lex.isEmpty())
        return nullptr;
    auto getToken = lex.getNextToken();
    if(getToken.isEqual("sin"))
    {
        makeUnaryOperation(node, lex, Rules :: FOURTEEN, std ::make_shared<SinFunction>(SinFunction()));
    }
    else if(getToken.isEqual("cos"))
    {
        makeUnaryOperation(node, lex, Rules :: FIVTEEN, std ::make_shared<CosFunction>(CosFunction()));
    }
    else if(getToken.isEqual("abs"))
    {
        makeUnaryOperation(node, lex, Rules :: SIXTEEN, std ::make_shared<AbsFunction>(AbsFunction()));
    }
    else if(getToken.isTypeVariable(CToken :: VARIABLE))
    {
        auto getToken = lex.getNextToken();
        node->m_rule = Rules :: SEVEN;
        if(getToken.getTypeValue() == CToken :: INT)
            node->m_type = std::make_shared<CInt>(getToken.getIntNameToken());
        else if(getToken.getTypeValue() == CToken :: STRING)
            node->m_type = std::make_shared<CString>(getToken.getStrNameToken());
        lex.popToken();

    }
    else if(getToken.isEqual("("))
    {
        lex.popToken();

        node->m_rule = Rules :: EIGHT;
        node->m_childs.push_back(std ::make_shared<Node>(Node(std ::make_shared<CString>("("))));
        node->m_childs.push_back(std ::make_shared<Node>(Node()));
        node->m_childs.back() = A( lex);
        if(lex.isEmpty() or ! lex.getNextToken().isEqual(")"))
            throw std :: exception();
        lex.popToken();

        node->m_childs.push_back( std :: make_shared<Node>(Node(std ::make_shared<CString>(")"))));
    }
    return node;
}


void CParser :: wrapperSyntaxTree(     std :: shared_ptr<CParser :: Node> node,
                                       std :: shared_ptr<ASTAbstractNode> & shod,
                                       std :: shared_ptr<ASTAbstractNode> &  dhod
                                 )
{
    if(node == nullptr)
        throw std :: exception();
    switch(node->m_rule)
    {
        case Rules::TWO  : { wrapperTwoRule(node, shod, dhod);  break; }
        case Rules::FOUR : { wrapperFourRule(node, shod, dhod); break; }
        case Rules::FIVE : { wrapperFiveRule(node, shod, dhod); break; }

        case Rules::SIX :
        case Rules::TEN :
        case Rules::ELEVEN:
        case Rules::TWELVE :
        case Rules::THIRTEEN : { wrapperThirteenRule(node, shod, dhod); break; }

        case Rules::SEVEN : { wrapperSevenRule(node, shod, dhod); break; }
        case Rules::EIGHT : { wrapperEightRule(node, shod, dhod); break; }
        case Rules::NINE :  { shod = dhod; break; }

        case Rules::FOURTEEN :
        case Rules::FIVTEEN :
        case Rules::SIXTEEN :
        case Rules::SEVENTEEN : { wrapperSeventeenRule(node, shod, dhod); break; }

        case Rules::EIGHTEEN :
        case Rules::NINETEEN : { wrapperNineteenRule(node, shod, dhod); break; }
        case Rules::TWENTY : { wrapperTwentyRule(node, shod, dhod); break; }
         default:
            throw std :: exception();
    }
}

void CParser :: wrapperTwoRule   (
                                    std::shared_ptr<Node> node,
                                    std::shared_ptr<ASTAbstractNode> &shod,
                                    std::shared_ptr<ASTAbstractNode> &dhod
                                )
{
    std :: shared_ptr<ASTAbstractNode> shodS_1 = nullptr;
    std :: shared_ptr<ASTAbstractNode> dhodS_1 = std ::make_shared<ASTLeafNode>(ASTLeafNode(node->m_childs[0]->m_type));
    wrapperSyntaxTree(node->m_childs[1], shodS_1, dhodS_1);
    shod = shodS_1;
}

void CParser :: wrapperFourRule  (
                                    std::shared_ptr<Node> node,
                                    std::shared_ptr<ASTAbstractNode> &shod,
                                    std::shared_ptr<ASTAbstractNode> &dhod
                                )
{
    std :: shared_ptr<ASTAbstractNode> shodA = nullptr;
    std :: shared_ptr<ASTAbstractNode> dhodA = nullptr;
    wrapperSyntaxTree(node->m_childs[0], shodA, dhodA);
    auto left = dhod;
    auto right = shodA;
    shod = std ::make_shared<ASTBinaryNode>(
            ASTBinaryNode(left, right, node->m_operBinary->clone()));
}

void CParser :: wrapperFiveRule  (
                                    std::shared_ptr<Node> node,
                                    std::shared_ptr<ASTAbstractNode> &shod,
                                    std::shared_ptr<ASTAbstractNode> &dhod
                                )
{
    std :: shared_ptr<ASTAbstractNode> shodE = nullptr;
    std :: shared_ptr<ASTAbstractNode> dhodE = nullptr;
    wrapperSyntaxTree(node->m_childs[0], shodE, dhodE);
    std :: shared_ptr<ASTAbstractNode> shodA = nullptr;
    wrapperSyntaxTree(node->m_childs[1], shodA, shodE);
    shod = shodA;
}


void CParser :: wrapperSevenRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    if(auto tempString = node->m_type->tryGetString(); tempString.has_value() && m_table->isPresentInTable(tempString.value()))
        shod = std ::make_shared<ASTLinkNode>(ASTLinkNode(tempString.value(), m_table));
    else
        shod = std::make_shared<ASTLeafNode>(node->m_type);
}

void CParser :: wrapperEightRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    wrapperSyntaxTree(node->m_childs[1],shod, dhod);
    shod = std ::make_shared<ASTBracket>(ASTBracket(shod));
}

void CParser :: wrapperThirteenRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    std :: shared_ptr<ASTAbstractNode> shodA = nullptr;

    wrapperSyntaxTree(node->m_childs[0], shodA, dhod);
    auto left = dhod;
    auto right = shodA;
    shod = std ::make_shared<ASTBinaryNode>(
            ASTBinaryNode(left, right, node->m_operBinary->clone()));
}

void CParser :: wrapperFourteenRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    std :: shared_ptr<ASTAbstractNode> shodA = nullptr;

    wrapperSyntaxTree(node->m_childs[0], shodA, dhod);
    auto left = dhod;
    auto right = shodA;
    shod = std ::make_shared<ASTBinaryNode>(
            ASTBinaryNode(left, right, node->m_operBinary->clone()));
}

void CParser :: wrapperSeventeenRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    wrapperSyntaxTree(node->m_childs[0],shod, dhod);
    shod = std ::make_shared<ASTUnaryFunction>(ASTUnaryFunction(shod, node->m_operUnary->clone()));
}

void CParser :: wrapperNineteenRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    shod = std ::make_shared<ASTUnaryFunction>(ASTUnaryFunction(std ::make_shared<ASTLeafNode>(ASTLeafNode(node->m_type)), node->m_operUnary->clone()));
}

void CParser :: wrapperTwentyRule  (
        std::shared_ptr<Node> node,
        std::shared_ptr<ASTAbstractNode> &shod,
        std::shared_ptr<ASTAbstractNode> &dhod
)
{
    shod = std ::make_shared<ASTUnaryFunction>(ASTUnaryFunction(shod, node->m_operUnary));
}

void CParser ::  makeBinaryOperation (
                                          std :: shared_ptr<CParser :: Node> & node,
                                          CLexan & lex,
                                          CParser :: Rules rule,
                                          std :: shared_ptr<BinaryFunction> func
                                      )
{
    lex.popToken();
    node->m_rule = rule;
    node->m_operBinary = func->clone();
    node->m_childs.push_back(A(lex));
}

void CParser :: makeUnaryOperation (std :: shared_ptr<CParser :: Node> & node,
                         CLexan & lex,
                         Rules rule,
                         std :: shared_ptr<UnaryFunction> func)
{
    lex.popToken();
    node->m_rule = rule;
    node->m_operUnary = func->clone();
    if(! (! lex.isEmpty() && lex.getNextToken().isEqual("(")))
        throw std :: exception();
    lex.popToken();
    node->m_childs.push_back(A(lex));
    if(! (! lex.isEmpty() && lex.getNextToken().isEqual(")")))
        throw std :: exception();
    lex.popToken();
}