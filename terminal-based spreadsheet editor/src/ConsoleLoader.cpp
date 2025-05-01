#include "ConsoleLoader.hpp"

std :: pair<bool, std :: string> ConsoleLoader :: load()
{
    std :: string res;
    auto & retOs = std ::getline(std :: cin, res);
    if(! retOs.good() || retOs.eof())
        return std ::make_pair(false, "");
    return std ::make_pair(true, res);
}

