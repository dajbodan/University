# test/test_LCF.py

from LCF import LCF


def test_basic_cases():
    x = "abracadabra"
    y = "cadabracad"

    positions, length = LCF(x, y)

    assert length == 7
    assert 4 in positions
    assert positions == [4]


def test_multiple_lcf_occurrences():
    x = "aaaaa"
    y = "aa"

    positions, length = LCF(x, y)

    assert length == 2
    assert positions == [0, 1, 2, 3]


def test_full_match_when_equal():
    x = "banana"
    y = "banana"

    positions, length = LCF(x, y)
    assert length == len(x)
    assert positions == [0]


def test_no_common_substring():
    x = "abc"
    y = "xyz"

    positions, length = LCF(x, y)
    assert positions == []
    assert length == 0


def test_empty_strings():
    assert LCF("", "abc") == ([], 0)
    assert LCF("abc", "") == ([], 0)
    assert LCF("", "") == ([], 0)


def test_multiple_longest_common_substrings():
    # Two possible longest substrings: "abc" and "bca" at three positions
    x = "abcabc"
    y = "bcaxyz"

    positions, length = LCF(x, y)
    assert length == 3
    assert positions == [0,1,3]
