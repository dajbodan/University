class State:
  """
  A node (state) of the suffix automaton.
  """


  def __init__(self):
    self.suffix_link = None
    self.next = {}


  def goto(self, c):
    """Return destination state id on character `c`, or None if absent."""
    return self.next.get(c, None)



class SuffixAutomaton:
  """
  Suffix Automaton for a single text T.
  Implement the following public methods:
    build(T) -> None
      Build the automaton from text T. Should reset previous content.
    count(P) -> int
      Return number of occurrences of `pattern` in the built text.
    match_first(P) -> int
      Starting index of the first (leftmost) occurrence, or -1 if absent.
    match_last(P) -> int
      Starting index of the last (rightmost) occurrence, or -1 if absent.
    match_all(P) -> list[int]
      Return all starting indices where `pattern` occurs in the text.
  """
  

  def __init__(self):
    self.states = [State()]
    self._alphabet = set()


  def n_states(self):
    """
    Get number of states in M.
    """
    raise NotImplementedError("TODO: implement n_states()")


  def n_transitions(self):
    """
    Get number of transitions in M.
    """
    raise NotImplementedError("TODO: implement n_transitions()")


  def build(self, T):
    """
    Build the automaton from text T.
    """
    raise NotImplementedError("TODO: implement build()")
  

  def count(self, P) -> int:
    """
    Number of occurrences of pattern P in the built text.
    """
    raise NotImplementedError("TODO: implement count()")
  

  def match_first(self, P) -> int:
    """
    Starting index of the first (leftmost) occurrence, or -1 if absent.
    """
    raise NotImplementedError("TODO: implement match_first()")
  

  def match_last(self, P) -> int:
    """
    Starting index of the last (rightmost) occurrence, or -1 if absent.
    """
    raise NotImplementedError("TODO: implement match_last()")
  

  def match_all(self, P) -> list:
    """
    Return all starting positions of pattern P in the text.
    """
    raise NotImplementedError("TODO: implement match_all()")
  