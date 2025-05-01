#include "FileLoader.hpp"

FileLoader :: FileLoader( const std :: string & path)
{
    std :: string prefixPath = "./examples/";
    std :: string format = ".txt";
    if(path.substr(0, prefixPath.size()) !=  prefixPath || path.substr(path.size() - format.size()) != format)
        throw std ::exception();
    m_fileInput = std :: ifstream();
    m_fileInput.open(path, std :: ios :: in);
    if(! m_fileInput )
        throw std :: exception();

}

std :: pair<bool, std :: string> FileLoader :: load()
{
    std :: string line = "";
    std ::getline(m_fileInput, line);
    if(! m_fileInput.good() && line == "")
        return std :: make_pair(false, "");
    return std ::make_pair(true, line);
}
