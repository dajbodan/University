// src/LCF.hpp
#pragma once

#include <utility>  
#include <vector>   
#include <string>
#include <memory>
#include "SAM.hpp"


/**
 * Longest Common Factors between strings x and y.
 *
 * @return pair: { list of start positions in x where LCF occurs, length }
 */
std::pair<std::vector<int>, int> LCF(const std::string& x, const std::string& y);
