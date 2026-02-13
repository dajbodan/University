// src/LCF.cpp
// Longest Common Factor

#include "LCF.hpp"
#include "SAM.hpp"
//#include <stdexcept>




std::pair<std::vector<int>, int> LCF(const std::string& x, const std::string& y) {
    SuffixAutomaton sa;
    sa.build(x);
    return sa.LCF(y);
}
