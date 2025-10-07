all: main

main: objects/main.o objects/lexer.o objects/token.o
	g++ objects/main.o objects/lexer.o objects/token.o -o main

objects/main.o: main.cpp | objects
	g++ -c main.cpp -o objects/main.o

objects/lexer.o: include/lexer.hpp src/lexer.cpp | objects
	g++ -c src/lexer.cpp -o objects/lexer.o

objects/token.o: include/token.hpp src/token.cpp | objects
	g++ -c src/token.cpp -o objects/token.o

objects:
	mkdir -p objects

.PHONY: clean
clean:
	rm -f objects/*.o main
