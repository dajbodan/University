// SAM.hpp
// Suffix Automaton

#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <numeric>
#include <memory>
#include <stdexcept>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <memory>

class State {
    // A node (state) of the suffix automaton.
    public:

    std :: shared_ptr<State> m_suffix_link;
    std::unordered_map<char, std :: shared_ptr<State>> m_next; // transitions
    std :: unordered_set<std :: shared_ptr<State>> m_reversed_suffix_links;
    bool m_is_clone = false;
    int m_count = -1;
    int m_length = -1;



    State() = default;
    ~State() = default;

    // Return destination state id on character `c`, or null if absent.
    std :: shared_ptr<State> gotoNext(char c);
    std :: shared_ptr<State> findState(const std :: string & P, int index, const std :: shared_ptr<State> & current)
    {
        if(index  >= (int)P.size())
            return current;
        return findState(P, index + 1, gotoNext(P[index]));
    }


};




class SuffixAutomaton {
private: 
    std::vector<std :: shared_ptr<State>> m_states;
    std::unordered_map<char, bool> alphabet;
    std :: shared_ptr<State> m_root = nullptr;
    std :: shared_ptr<State> m_last = nullptr;
    std :: vector<std :: shared_ptr<State>> m_finalStates;
  // Suffix Automaton class.
public:
    SuffixAutomaton();
    ~SuffixAutomaton();
    
    int n_states() const;
    // Get number of states in M.

    int n_transitions() const;
    // Get number of transitions in M.

    // Build the automaton from text T. Should reset previous content.
    void build(const std::string& T);
    
    // Number of occurrences of P in the built text.
    int count(const std::string& P) const;
    
    // Starting index of the first (leftmost) occurrence, or -1 if absent.
    int  match_first(const std::string& P) const;
    
    // Starting index of the last (rightmost) occurrence, or -1 if absent.
    int  match_last (const std::string& P) const;

    // Return all starting indices where P occurs in the text.
    std::vector<int> match_all(const std::string& P) const;

    std::pair<std::vector<int>, int> LCF(const std::string& T) const;
private :
    void extension(char letter);
    void set_suffix_link(std :: shared_ptr<State> , std :: shared_ptr<State> );
    void initializeCounter(std :: shared_ptr<State>);
    std :: vector<int> findAllIndexes(std :: shared_ptr<State>) const;
    std :: shared_ptr<State> findState(const std :: string & ) const;
};
