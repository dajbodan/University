#pragma once

#include "TableSaver.hpp"
#include <sstream>
#include <iterator>
#include <fstream>

/**
 * this class represent save of table into file
 */
class FileSaver : public TableSaver
{
public :
    FileSaver(const std :: string & src);
    bool save(const std :: shared_ptr<Table> & table) override;
private :
    std :: ofstream m_fileOutput;
};