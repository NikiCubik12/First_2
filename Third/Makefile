all: program tests

program: main.o
	g++ -Wall -g -std=c++17 -o program main.o 

tests: Run_Tests.o
	g++ -Wall -g -std=c++17 -o tests Run_Tests.o

main.o: main.cpp Complex.hpp DynamicArray.hpp LinkedList.hpp Sequence.hpp TriangleMatrix.hpp
	g++ -Wall -g -std=c++17 -c main.cpp

Run_Tests.o: Run_Tests.cpp Tests.hpp DynamicArray.hpp LinkedList.hpp Sequence.hpp
	g++ -Wall -g -std=c++17 -c Run_Tests.cpp

clean:
	rm -f main.o Run_Tests.o program tests

run: program
	./program

test: tests
	./tests