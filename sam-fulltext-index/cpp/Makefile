# ===== Makefile for Suffix Automaton Project =====

# Tools & flags
CXX       := g++
CXXFLAGS  := -Wall -Wextra -O2
# Put include dirs in CPPFLAGS (and define SRC_DIR before using it!)
SRC_DIR   := src
TEST_DIR  := test
BIN_DIR   := bin
CPPFLAGS  := -I$(SRC_DIR)

# Library sources/objects
SRCS := $(SRC_DIR)/SAM.cpp $(SRC_DIR)/LCF.cpp
OBJS := $(SRCS:.cpp=.o)

# Auto-discover tests: test/test_*.cpp -> bin/test_*
TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_BINS := $(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))

.PHONY: all test clean

all: $(TEST_BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile library sources to .o (no linking)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Link each test executable against the objs (compiles the test on the fly)
$(BIN_DIR)/%: $(TEST_DIR)/%.cpp $(OBJS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) $< -o $@

test: all
	@for t in $(TEST_BINS); do \
		echo "Running $$t..."; \
		"$$t" || exit $$?; \
	done

clean:
	rm -rf $(BIN_DIR) $(OBJS)
