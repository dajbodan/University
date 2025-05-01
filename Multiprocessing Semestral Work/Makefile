CXX=g++ -std=c++17
LD=g++ -std=c++17
AR=ar
CXXFLAGS=-std=c++17 -Wall -pedantic -O2
SHELL:=/bin/bash
MACHINE=$(shell uname -m)-$(shell echo $$OSTYPE)


all: test

deps:
	g++ -MM *.cpp > Makefile.d

test: solution.o sample_tester.o
	$(LD) $(CXXFLAGS) -o $@ $^ -L./arm64-darwin22 -lprogtest_solver -lpthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

lib: progtest_solver.o
	mkdir -p $(MACHINE)
	$(AR) cfr $(MACHINE)/libprogtest_solver.a $^

clean:
	rm -f *.o test *~ core sample.tgz Makefile.d
	
pack: clean
	rm -f sample.tgz
	tar zcf sample.tgz --exclude progtest_solver.cpp --exclude Makefile.mingw *
	

-include Makefile.d
