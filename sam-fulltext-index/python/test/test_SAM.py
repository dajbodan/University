# test/test_SAM.py

from SAM import SuffixAutomaton


# --- Basic: build + simple queries ------------------------------------

def test_build_and_basic_counts_abracadabra():
    sa = SuffixAutomaton()
    sa.build("abracadabra")

    # P in T
    assert sa.count("abra") == 2
    assert sa.count("ra") == 2
    assert sa.count("cad") == 1
    assert sa.count("adab") == 1

    # P not in T 
    for p in ["xyz", "abba", "aaab"]:
        assert sa.count(p) == 0


def test_match_positions_abracadabra():
    sa = SuffixAutomaton()
    sa.build("abracadabra")

    assert sa.match_first("abra") == 0
    assert sa.match_last("abra") == 7
    assert sa.match_all("abra") == [0, 7]

    assert sa.match_first("ra") == 2
    assert sa.match_last("ra") == 9
    assert sa.match_all("ra") == [2, 9]

    assert sa.match_first("cad") == 4
    assert sa.match_last("cad") == 4
    assert sa.match_all("cad") == [4]


# --- Overlaps ----------------------------------------------

def test_overlaps_multiple_occurrences():
    T = "aaaaa"
    sa = SuffixAutomaton()
    sa.build(T)

    assert sa.count("a") == 5
    assert sa.match_first("a") == 0
    assert sa.match_last("a") == 4
    assert sa.match_all("a") == [0, 1, 2, 3, 4]

    assert sa.count("aa") == 4
    assert sa.match_all("aa") == [0, 1, 2, 3]

    assert sa.count("aaa") == 3
    assert sa.match_all("aaa") == [0, 1, 2]
    assert sa.match_first("aaa") == 0
    assert sa.match_last("aaa") == 2


# --- Empty ------------------------------------------

def test_empty_text_behavior():
    sa = SuffixAutomaton()
    sa.build("")

    p = "aa"
    assert sa.count(p) == 0
    assert sa.match_first(p) == -1
    assert sa.match_last(p) == -1
    assert sa.match_all(p) == []


def test_empty_pattern_convention():
    sa = SuffixAutomaton()
    sa.build("abcde")

    assert sa.count("") == 0
    assert sa.match_first("") == -1
    assert sa.match_last("") == -1
    assert sa.match_all("") == []


# --- Rebuild (reset) -------------------------------------------------------

def test_rebuild_resets_state():
    sa = SuffixAutomaton()
    sa.build("abcabc")
    c1 = sa.count("abc")
    mf1 = sa.match_first("abc")
    ml1 = sa.match_last("abc")

    sa.build("zzzz")
    assert sa.count("abc") == 0
    assert sa.match_first("abc") == -1
    assert sa.match_last("abc") == -1

    assert sa.count("zz") == 3
    assert sa.match_all("zz") == [0, 1, 2]
    assert sa.match_first("zz") == 0
    assert sa.match_last("zz") == 2

    assert (sa.count("abc") != c1) or (sa.match_first("abc") != mf1) or (sa.match_last("abc") != ml1)


# --- Other ----------------------------------------------

def test_sentence_varied_alphabet():
    T = "the quick brown fox jumps over the lazy dog"
    sa = SuffixAutomaton()
    sa.build(T)

    assert sa.count("the") == 2
    assert sa.match_all("the") == [0, 31]
    assert sa.match_first("the") == 0
    assert sa.match_last("the") == 31

    assert sa.count("fox") == 1
    pos = T.index("fox")
    assert sa.match_first("fox") == pos
    assert sa.match_last("fox") == pos

    assert sa.count("doge") == 0
    assert sa.match_all("doge") == []
    assert sa.match_first("doge") == -1
    assert sa.match_last("doge") == -1

def test_states_and_transitions():
    # --- abracadabra ---
    T = "abracadabra"
    n = len(T)
    sa = SuffixAutomaton()
    sa.build(T)

    states = sa.n_states()
    transitions = sa.n_transitions()
    print(f"[abracadabra] states={states}, transitions={transitions}")

    assert states > 0
    assert states <= 2 * n - 1
    assert transitions <= 3 * n - 4  # classical upper bound

    # --- aaaaa ---
    T = "aaaaa"
    n = len(T)
    sa = SuffixAutomaton()
    sa.build(T)

    states = sa.n_states()
    transitions = sa.n_transitions()
    print(f"[aaaaa] states={states}, transitions={transitions}")

    assert states <= 2 * n - 1
    assert transitions <= 3 * n - 4

    # --- empty ---
    T = ""
    sa = SuffixAutomaton()
    sa.build(T)
    states = sa.n_states()
    transitions = sa.n_transitions()
    print(f"[empty] states={states}, transitions={transitions}")

    assert states == 1           # only the root state
    assert transitions == 0      # no edges