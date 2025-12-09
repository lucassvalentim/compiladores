CXXFLAGS = -g -O0 -Wall -Wextra 

all: main

main: objects/main.o objects/lexer.o objects/token.o objects/parser.o objects/symbol_table.o objects/asa.o
	g++ $(CXXFLAGS) $^ -o main

objects/main.o: main.cpp | objects
	g++ $(CXXFLAGS) -c main.cpp -o objects/main.o

objects/lexer.o: include/lexer.hpp src/lexer.cpp | objects
	g++ $(CXXFLAGS) -c src/lexer.cpp -o objects/lexer.o

objects/token.o: include/token.hpp src/token.cpp | objects
	g++ $(CXXFLAGS) -c src/token.cpp -o objects/token.o

objects/parser.o: include/parser.hpp src/parser.cpp | objects
	g++ $(CXXFLAGS) -c src/parser.cpp -o objects/parser.o

objects/symbol_table.o: include/symbol_table.hpp src/symbol_table.cpp | objects
	g++ $(CXXFLAGS) -c src/symbol_table.cpp -o objects/symbol_table.o

objects/asa.o: include/asa.hpp src/asa.cpp | objects
	g++ $(CXXFLAGS) -c src/asa.cpp -o objects/asa.o

objects:
	mkdir -p objects

.PHONY: clean
clean:
	rm -f objects/*.o main
