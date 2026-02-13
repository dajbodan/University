#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include <numeric>
#include <iomanip>
#include <sstream>
#include "SAM.hpp"
#include "LCF.hpp"
#include <cassert>

std :: string readFile(const std :: filesystem::path & path)
{
    std :: ifstream file(path);
    if(!file)
        throw std :: runtime_error("Could not open file " + path.string());
    std :: ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

std :: vector<std :: string> patternParser(const std :: string & src, char delim)
{
    std :: vector<std :: string> res;

    std :: stringstream ss(src);
    for(std :: string line; std ::getline(ss, line, delim);)
        res.push_back(line);

    return res;
}


/*
 * Important assert(all_matches.size() == count) which checks whether all matched patterns that we found, match with occurence
 */
void runExperiment(
            const std :: filesystem::path & textPath,
            const std :: filesystem::path & patternPath,
            const std :: filesystem::path & outputPath
        )
{
    std :: vector<std :: string> patterns;
    std :: string inputText;
    try {
         patterns = patternParser(readFile(patternPath), '\n');
         inputText = readFile(textPath);
    }
    catch (const std :: exception & e)
    {
        std :: cerr << e.what() << "\n";
        return;
    }
    SuffixAutomaton sa;
    auto time_build = std :: chrono::high_resolution_clock::now();
    sa.build(inputText);
    auto end_build = std :: chrono::high_resolution_clock::now();
    long long duration_build = std::chrono::duration_cast<std::chrono::nanoseconds>(end_build - time_build).count();
    std :: ofstream outputFile(outputPath);
    outputFile << "length,occurrences,time[ns],states,transitions,time_build\n";
    bool beginFlag = true;
    for(auto it : patterns)
    {
        long long count = sa.count(it);
        auto begin = std :: chrono::high_resolution_clock::now();
        auto all_matches = sa.match_all(it);
        auto end = std :: chrono::high_resolution_clock :: now();
        assert(all_matches.size() == count);
        long long duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        if(beginFlag)
        {
            outputFile << it.length() << "," << count << "," << duration << ","  << sa.n_states() << "," << sa.n_transitions()<< "," << duration_build << "\n";
            beginFlag = false;
        }
        else
        {
            outputFile << it.length() << "," << count << "," << duration << ",-,-,-"  <<"\n";
        }
    }
}


int main()
{
    std :: filesystem::path dir = "./datasets/";
    std :: vector<std :: string> targets = { "dna", "english","mono", "random"};

    for(auto it : targets)
    {
        auto textPath     = dir / it / (it + "1MB.txt");
        auto patternsPath = dir / it / (it + "1MB_patterns.txt");
        auto saveResultsPath = dir / it / (it + "SAM_results.csv");


        runExperiment(textPath, patternsPath, saveResultsPath);

    }
    return 0;
}