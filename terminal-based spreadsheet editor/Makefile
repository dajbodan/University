CXX=g++
LD = g++
STANDARD=-std=c++17
FLAGS=-Wall -pedantic -g
srcFiles=src
build=build
doxFile=doc

SRC=$(wildcard src/*.cpp)
HDR=$(wildcard src/*.h)
OBJ=$(patsubst $(srcFiles)/%.cpp, $(build)/%.o, $(SRC))

all : dajbodan


run : dajbodan
	./dajbodan

dajbodan : $(OBJ)
	$(LD) $(FLAGS) $^ -o $@

compile : $(OBJ)
	$(LD) $(STANDARD) $(build)/*.o -o dajbodan

$(build)/%.o : $(srcFiles)/%.cpp
	$(CXX) $(STANDARD) $(FLAGS)  -c -o $@ $<

doc :
	doxygen Doxyfile

clean :
	rm -f $(build)/*.o
	rm -f dajbodan
	rm -rf $(doxFile)

