#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../src/LCF.hpp"


void test_basic_cases() {
    std::string x = "abracadabra";
    std::string y = "cadabracad";

    auto result = LCF(x, y);
    const auto& positions = result.first;
    int length = result.second;
    assert(length == 7);
    assert(std::find(positions.begin(), positions.end(), 4) != positions.end());
    /*assert((positions == std::vector<int>{4}));
     * Podezreni na chybu v testech protoze melo by byt {0,4}, protoze pro x a y je nejvetsi subtring "abracad" zacinajici v 0 pro x a v 3 pro y a
     * "cadabra" v 4 pro a v 0 pro y
    */
    assert((positions == std::vector<int>{0, 4}));  // Tohle by melo byt spravne
}

void test_multiple_lcf_occurrences() {
    std::string x = "aaaaa";
    std::string y = "aa";

    auto [positions, length] = LCF(x, y);

    assert(length == 2);
    assert((positions == std::vector<int>{0, 1, 2, 3}));
}

void test_full_match_when_equal() {
    std::string x = "banana";
    std::string y = "banana";

    auto [positions, length] = LCF(x, y);

    assert(length == static_cast<int>(x.size()));
    assert((positions == std::vector<int>{0}));
}

void test_no_common_substring() {
    std::string x = "abc";
    std::string y = "xyz";

    auto [positions, length] = LCF(x, y);

    assert(length == 0);
    assert(positions.empty());
}

void test_empty_strings() {
    {
        auto [positions, length] = LCF("", "abc");
        assert(length == 0);
        assert(positions.empty());
    }
    {
        auto [positions, length] = LCF("abc", "");
        assert(length == 0);
        assert(positions.empty());
    }
    {
        auto [positions, length] = LCF("", "");
        assert(length == 0);
        assert(positions.empty());
    }
}

void test_multiple_longest_common_substrings() {
    std::string x = "abcabc";
    std::string y = "bcaxyz";

    auto [positions, length] = LCF(x, y);

    assert(length == 3);
    //assert((positions == std::vector<int>{0, 1, 3}));
    /*
     * Podezreni na chybu. substring "abc" neni pritomen v y, jak se to tvrdi v .py testech pro LCF
     * Je pritomen bca zacinajici na indexu 1 pro x a na indexu 0 pro x
     */
    assert((positions == std::vector<int>{1})); // Tohle melo by byt spravne
}

int main() {
    std::cout << "Running LCF tests...\n";

    test_basic_cases();
    test_multiple_lcf_occurrences();
    test_full_match_when_equal();
    test_no_common_substring();
    test_empty_strings();
    test_multiple_longest_common_substrings();

    std::cout << "✅ All LCF tests passed successfully.\n";
    return 0;
}
