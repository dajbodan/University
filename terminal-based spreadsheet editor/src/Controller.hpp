#pragma once

#include <optional>
#include <stack>
#include "FileLoader.hpp"
#include "FileSaver.hpp"
#include "ConsoleLoader.hpp"
#include "CLexan.hpp"
#include "CParser.hpp"
#include "AbstractSyntaxTree.hpp"
#include "TableSaver.hpp"
#include "TableLoader.hpp"

#include "Table.hpp"

/**
 * this class is reponsible for running application
 */
class Controller
{
public :
    /**
     * it creates singleton variable
     */
    Controller(const std :: shared_ptr<ConsoleLoader> loader) : m_table(std ::make_shared<Table>(Table())) { m_loaders.push(std ::make_shared<ConsoleLoader>( * loader)); }
    /**
     * this method runs application
     */
    void startApplication();


private :
    /**
     * this method change a way of loading new statements
     * @param path represent path to file
     * @return
     */
    bool loadFile(const std :: string & path);

    std :: stack<std :: shared_ptr<TableLoader>> m_loaders;
    std :: shared_ptr<Table> m_table;

};