#include "FileSaver.hpp"

FileSaver :: FileSaver(const std :: string & src)
{
    std :: string prefixPath = "./examples/";
    std :: string format = ".txt";

    if(src.substr(0, prefixPath.size()) != prefixPath || src.substr(src.size() - format.size()) != format)
        throw std :: exception();
    m_fileOutput.open(src, std :: ios :: out);


    if( ! m_fileOutput ) {
        std :: cout << "I am here\n";
        throw std::exception();
    }
}


bool FileSaver ::save(const std :: shared_ptr<Table> &table)
{
    if(!m_fileOutput)
        return false;
    auto formulas = table->convertToFormula();
    if(formulas.empty())
        return false;
    std :: copy(formulas.begin(), formulas.end(), std ::ostream_iterator<std :: string>(m_fileOutput));
    return true;
}