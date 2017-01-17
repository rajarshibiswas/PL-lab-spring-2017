
Interpreter: interpreter.o scanner.o
	g++ interpreter.o scanner.o -o Interpreter

interpreter.o: interpreter.cpp common.h
	g++ -std=c++11 -c interpreter.cpp

scanner.o:  scanner.cpp scanner.h common.h
	g++ -std=c++11 -c scanner.cpp
clean:
	rm *.o Interpreter

