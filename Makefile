CPP  = g++
CC   = gcc
OBJ  = dfa.o nfa.o regexp.o test.o 
LINKOBJ  = dfa.o nfa.o regexp.o test.o 
BIN  = RegExp
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before RegExp all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "RegExp"

dfa.o: dfa.cpp
	$(CPP) -g -c dfa.cpp -o dfa.o 

nfa.o: nfa.cpp
	$(CPP) -g -c nfa.cpp -o nfa.o 

regexp.o: regexp.cpp
	$(CPP) -g -c regexp.cpp -o regexp.o 

test.o: test.cpp
	$(CPP) -g -c test.cpp -o test.o 
