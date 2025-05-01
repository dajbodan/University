#include "Controller.hpp"

void Controller ::  startApplication()
{

    while(1)
    {
        if(m_loaders.empty())
            break;
        auto getValue = m_loaders.top()->load();
        if(! getValue.first)
        {
            if(m_loaders.size() == 1)
                break;
            m_loaders.pop();
            continue;
        }
        CLexan lex;
        if(! lex.readInput(getValue.second))
        {
            std::cerr << "lex Error\n";
            continue;
        }
        std ::shared_ptr<CParser> ptrTree = nullptr;
        AbstractSyntaxTree ast;
        try
        {
            ptrTree = std ::make_shared<CParser>(CParser(lex, m_table, std :: bind(& Controller ::loadFile, this, std :: placeholders:: _1)));
            ptrTree->makeAst(ast);
        }
        catch (std :: exception & ex) { std :: cerr << "parse error\n"; continue; }
        if(ast.isCycleDetected())
        {
            std :: cerr << "Cycle detected. Statement is cancelled\n";
            continue;
        }
        if(ast.evaluate() == nullptr)
            std :: cerr << "evaluation Error\n";
    }
}

bool Controller :: loadFile(const std :: string & path)
{
    try {
        m_loaders.push(std::make_shared<FileLoader>(FileLoader(path)));
    }
    catch(std :: exception & ex)
    {
        std :: cerr << "Cannot load" << "\n";
    }
    return true;
}