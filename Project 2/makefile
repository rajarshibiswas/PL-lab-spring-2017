
Interpreter: src/interpreter.o src/scanner.o src/parser.o
	g++ src/interpreter.o src/scanner.o src/parser.o -o Interpreter

interpreter.o: src/interpreter.cpp src/common.h
	g++ -std=c++11 -c src/interpreter.cpp

scanner.o:  src/scanner.cpp src/scanner.h src/common.h
	g++ -std=c++11 -c src/scanner.cpp

parser.o: src/parser.cpp src/parser.h src/common.h
	g++ -std=c++11 -c src/parser.cpp

clean:
	rm src/*.o Interpreter
