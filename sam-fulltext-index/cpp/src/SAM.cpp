// SAM.cpp
#include "SAM.hpp"



// ---------------- State ----------------

std :: shared_ptr<State> State::gotoNext(char c) {
    auto it = m_next.find(c);
    return (it == m_next.end()) ? NULL : it->second;
}

// ---------------- SuffixAutomaton ----------------

SuffixAutomaton::SuffixAutomaton() = default;
SuffixAutomaton::~SuffixAutomaton() = default;



int SuffixAutomaton::n_states() const{
    return m_states.size();
}

int SuffixAutomaton::n_transitions() const{
    return std :: accumulate(m_states.begin(), m_states.end(), 0, [](int partialResult, const std :: shared_ptr<State> & state)
    {
        return partialResult + state->m_next.size();
    });
}

void SuffixAutomaton::build(const std::string& T) {
    m_root = std ::make_shared<State>(State());
    m_states.push_back(m_root);
    m_root->m_length = 0;
    m_root->m_suffix_link = nullptr;
    m_root->m_count = 1;

    m_last = m_root;
    for(int i = 0; i < (int)T.size(); i++)
        extension(T[i]);
    auto it = m_last;
    while(it != nullptr)
    {
        m_finalStates.push_back(it);
        it = it->m_suffix_link;
    }
    for(auto& s : m_states) {
        s->m_reversed_suffix_links.clear();
    }
    for(auto& s : m_states) {
        if (s->m_suffix_link != nullptr) {
            s->m_suffix_link->m_reversed_suffix_links.insert(s);
        }
    }
    initializeCounter(m_root);
}

int SuffixAutomaton::count(const std::string& P) const {
    if(m_root == nullptr)
        throw std :: runtime_error("unitialized root in SA");
    auto res = findState( P);
    if(res == nullptr or res == m_root)
        return 0;
    return res->m_count;
}

int SuffixAutomaton::match_first(const std::string& P) const {
    auto all_matches = match_all(P);
    auto it_min = std ::min_element(all_matches.begin(), all_matches.end());
    return it_min != all_matches.end() ? *it_min : -1;
}

int SuffixAutomaton::match_last(const std::string& P) const {
    auto all_matches = match_all(P);
    auto it_max = std ::max_element(all_matches.begin(), all_matches.end());
    return it_max != all_matches.end() ? *it_max : -1;

}

std::vector<int> SuffixAutomaton::match_all(const std::string& P) const {
    if(m_root == nullptr)
        throw std :: runtime_error("unitialized root in SA");
    auto state = findState(P);
    std :: unordered_set<std :: shared_ptr<State>> visited;
    std :: vector<int> res = findAllIndexes(state);
    std ::transform(res.cbegin(), res.cend(), res.begin(), [&P](int index){ return index - P.size(); });
    return state == m_root ? std :: vector<int>() : res;
}


void SuffixAutomaton::extension(char letter)
{
    auto newState = std :: make_shared<State>();
    m_states.push_back(newState);
    newState->m_length = m_last->m_length + 1;
    newState->m_count = 1;
    newState->m_is_clone = false;
    auto last = m_last;
    while(last != nullptr and last->m_next.find(letter) == last->m_next.end())
    {
        last->m_next[letter] = newState;
        last = last->m_suffix_link;
    }

    if(last == nullptr)
    {
        set_suffix_link(newState, m_root);
    }
    else
    {
        auto q = last->m_next[letter];
        if(last->m_length + 1 == q->m_length)
        {

            set_suffix_link(newState, q);
        }
        else
        {
            auto clone = std :: make_shared<State>();
            m_states.push_back(clone);
            clone->m_length = last->m_length + 1;
            clone->m_next = q->m_next;
            clone->m_count = 0;
            clone->m_is_clone = true;

            set_suffix_link(newState, clone);

            set_suffix_link(clone, q->m_suffix_link);

            set_suffix_link(q, clone);

            while(last != nullptr and last->m_next[letter] == q)
            {
                last->m_next[letter] = clone;
                last = last->m_suffix_link;
            }
        }
    }
    m_last = newState;
}


void SuffixAutomaton :: set_suffix_link(std :: shared_ptr<State> from, std :: shared_ptr<State> to)
{
    from->m_suffix_link = to;
}


void SuffixAutomaton:: initializeCounter(std :: shared_ptr<State> state)
{
    std :: stack<std :: pair<std :: shared_ptr<State>, bool>> stackSimulation;
    stackSimulation.push({state, false});


    while(!stackSimulation.empty())
    {

        auto it = stackSimulation.top();
        stackSimulation.pop();


        if(!it.second)
        {
            stackSimulation.push({it.first, true});
            for (auto child: it.first->m_reversed_suffix_links)
                if (child != nullptr)
                    stackSimulation.push({child, false});
        }
        else
        {
            for (const auto& child : it.first->m_reversed_suffix_links)
            {
                if (child != nullptr)
                {
                    it.first->m_count += child->m_count;
                }
            }
        }
    }
}

std :: vector<int> SuffixAutomaton :: findAllIndexes(std :: shared_ptr<State> state) const
{

    if(state == nullptr)
        return {};
    std :: stack<std :: shared_ptr<State>> stackSimulation;
    std::unordered_set<std::shared_ptr<State>> visited;
    std :: vector<int> collectedIndexes;
    stackSimulation.push(state);
    visited.insert(state);
    while(!stackSimulation.empty())
    {
        auto top = stackSimulation.top();
        stackSimulation.pop();

        if(!top->m_is_clone) {
            collectedIndexes.push_back(top->m_length);
        }

        for(auto child : top->m_reversed_suffix_links) {
            if (child != nullptr and visited.find(child) == visited.end()) {
                stackSimulation.push(child);
                visited.insert(child);
            }
        }

    }
    return collectedIndexes;
}

std::shared_ptr<State> SuffixAutomaton :: findState( const std :: string & P) const
{
    auto current = m_root;
    for (char c : P) {
        current = current->gotoNext(c);

        if (current == nullptr) {
            return nullptr;
        }
    }
    return current;
}

std::pair<std::vector<int>, int> SuffixAutomaton::LCF(const std::string& T) const
{
    if (m_root == nullptr) return {{}, 0};

    auto v = m_root;
    int l = 0;
    int bestLen = 0;
    std :: vector<int> bestEndPositions;

    for (int i = 0; i < (int)T.size(); ++i) {
        while (v != m_root && v->gotoNext(T[i]) == nullptr) {
            v = v->m_suffix_link;
            l = v->m_length;
        }

        if (v->gotoNext(T[i])) {
            v = v->gotoNext(T[i]);
            l++;
        }

        if (l > bestLen) {
            bestLen = l;
            bestEndPositions.clear();
            bestEndPositions.push_back(i);
        }
        else if(l == bestLen and l > 0)
        {
            bestEndPositions.push_back(i);
        }
    }
    if(bestLen == 0)
        return {{}, 0};
    std::set<std::string> uniqueSubstrings;
    for (int endPos : bestEndPositions) {
        uniqueSubstrings.insert(T.substr(endPos - bestLen + 1, bestLen));
    }
    std :: vector<int> res;
    for( auto it : uniqueSubstrings)
    {
        auto temp = match_all(it);
        std :: copy(temp.begin(), temp.end(), std ::back_inserter(res));
    }
    return { res, bestLen};
}