#pragma once

#include "TableLoader.hpp"
#include <iostream>
#include <vector>
#include <fstream>


/**
 * this class represent a load table from file
 */
class FileLoader : public TableLoader
{
public :

    FileLoader( const std :: string & path);

    /**
    * @return true and string when this method reads from input successfully. Otherwise false and dummy string
    */
    std :: pair<bool, std :: string> load() override;


private:
    std :: ifstream m_fileInput;
    std :: vector<std :: string> m_lines;

};