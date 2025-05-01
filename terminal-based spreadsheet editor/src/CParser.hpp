#pragma once


#include "AbstractType.hpp"
#include "CLexan.hpp"
#include "AbstractSyntaxTree.hpp"
#include "UnaryFunction.hpp"
#include "PrintTableFunction.hpp"
#include "PrintFunction.hpp"
#include "LoadFunction.hpp"
#include "SaveFileFunction.hpp"
#include "CosFunction.hpp"
#include "SinFunction.hpp"
#include "AbsFunction.hpp"
#include "BinaryFunction.hpp"
#include "PlusFunction.hpp"
#include "MultiplyFunction.hpp"
#include "MinusFunction.hpp"
#include "DivideFunction.hpp"
#include "Concat.hpp"
#include "AssignOperator.hpp"
#include "ASTBinaryNode.hpp"
#include "ASTUnaryFunction.hpp"
#include "ASTLeafNode.hpp"
#include "ASTLinkNode.hpp"
#include "ASTBracket.hpp"
#include "ASTAbstractNode.hpp"
#include "Table.hpp"
#include "CString.hpp"
#include "CInt.hpp"
#include <variant>
#include <vector>
#include <utility>
#include <optional>
#include <functional>
#include <cstddef>

/**
 * This class builds ast and accept string on input, which is at first parsed by Lexan.
 */
class CParser
{
    enum Rules {
         TWO, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, ELEVEN, TWELVE, THIRTEEN,
        FOURTEEN, FIVTEEN, SIXTEEN, SEVENTEEN, EIGHTEEN, NINETEEN, TWENTY, UNKNOWN
    };
public :
    /**
     *
     * @param lex is lexer
     * @param table represents table where is stored variable
     */
    CParser(CLexan & lex,
            std :: shared_ptr<Table> table,
            std :: function<void(const std :: string &)> loaderFunction = nullptr );
    /**
     *
     * @param outTree is ast
     * @return wether making ast is succesfull or not
     */
    bool  makeAst(AbstractSyntaxTree & outTree);

private :
    struct Node
    {
        Node() : m_type(nullptr), m_operBinary(nullptr), m_rule(Rules :: UNKNOWN) { }
        Node(std :: shared_ptr<AbstractType> type) : m_type(type), m_operBinary(nullptr), m_operUnary(nullptr) { }
        Node(std :: shared_ptr<BinaryFunction> operBin) : m_type(nullptr), m_operBinary(operBin), m_operUnary(nullptr) { }
        Node(std :: shared_ptr<UnaryFunction> operUnar) : m_type(nullptr), m_operBinary(nullptr), m_operUnary(operUnar) { }


        std :: vector<std :: shared_ptr<Node> > m_childs;
        std :: shared_ptr<AbstractType> m_type;
        std :: shared_ptr<BinaryFunction> m_operBinary;
        std :: shared_ptr<UnaryFunction>  m_operUnary;

        Rules m_rule;
    };
    /**
     * this method represents notterminal symbol in grammar
     * @param lex
     * @return node of derived tree
     */
    std::shared_ptr<Node> S(CLexan &lex);
    /**
     * this method represents notterminal symbol in grammar
     * @param lex
     * @return node of derived tree
     */
    std::shared_ptr<Node> S_1(CLexan &lex);
    /**
     * this method represents notterminal symbol in grammar
     * @param lex
     * @return node of derived tree
     */
    std::shared_ptr<Node> A(CLexan &lex);
    /**
     * this method represents notterminal symbol in grammar
     * @param lex
     * @return node of derived tree
     */
    std::shared_ptr<Node> A_1(CLexan &lex);
    /**
     * this method represents notterminal symbol in grammar
     * @param lex
     * @return node of derived tree
     */
    std::shared_ptr<Node> E(CLexan &lex);

    /**
     *
     * @param node of derived tree
     * @param shod synthesize attribute
     * @param dhod inherited atrribute
     */
    void wrapperSyntaxTree(
                           std :: shared_ptr<Node> node,
                           std :: shared_ptr<ASTAbstractNode> & shod,
                           std :: shared_ptr<ASTAbstractNode> & dhod
                           );
    /**
     * this methid reprsents second rule of evaluating
     * @param node
     * @param shod
     * @param dhod
     */
    void wrapperTwoRule  (
                                std :: shared_ptr<Node> node,
                                std :: shared_ptr<ASTAbstractNode> & shod,
                                std :: shared_ptr<ASTAbstractNode> & dhod
                            );
    void wrapperFourRule  (
            std :: shared_ptr<Node> node,
            std :: shared_ptr<ASTAbstractNode> & shod,
            std :: shared_ptr<ASTAbstractNode> & dhod
    );
    void wrapperFiveRule  (
            std :: shared_ptr<Node> node,
            std :: shared_ptr<ASTAbstractNode> & shod,
            std :: shared_ptr<ASTAbstractNode> & dhod
    );

    void wrapperSevenRule  (
            std :: shared_ptr<Node> node,
            std :: shared_ptr<ASTAbstractNode> & shod,
            std :: shared_ptr<ASTAbstractNode> & dhod
    );
    void wrapperEightRule  (
            std :: shared_ptr<Node> node,
            std :: shared_ptr<ASTAbstractNode> & shod,
            std :: shared_ptr<ASTAbstractNode> & dhod
    );

    void wrapperThirteenRule  (
            std :: shared_ptr<Node> node,
            std :: shared_ptr<ASTAbstractNode> & shod,
            std :: shared_ptr<ASTAbstractNode> & dhod
    );
    void wrapperFourteenRule  (
            std::shared_ptr<Node> node,
            std::shared_ptr<ASTAbstractNode> &shod,
            std::shared_ptr<ASTAbstractNode> &dhod
    );

    void wrapperSeventeenRule  (
            std::shared_ptr<Node> node,
            std::shared_ptr<ASTAbstractNode> &shod,
            std::shared_ptr<ASTAbstractNode> &dhod
    );

    void wrapperNineteenRule  (
            std::shared_ptr<Node> node,
            std::shared_ptr<ASTAbstractNode> &shod,
            std::shared_ptr<ASTAbstractNode> &dhod
    );

    void wrapperTwentyRule  (
            std::shared_ptr<Node> node,
            std::shared_ptr<ASTAbstractNode> &shod,
            std::shared_ptr<ASTAbstractNode> &dhod
    );
    void makeBinaryOperation (std :: shared_ptr<CParser :: Node> & node,
                              CLexan & lex,
                              Rules rule,
                              std :: shared_ptr<BinaryFunction> func);
    void makeUnaryOperation (std :: shared_ptr<CParser :: Node> & node,
                              CLexan & lex,
                              Rules rule,
                              std :: shared_ptr<UnaryFunction> func);

    std :: shared_ptr<Node> m_root;
    std :: shared_ptr<Table> m_table;
    std :: optional<std :: function<void (const std :: string &)> > m_loader;

};