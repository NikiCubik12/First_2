all: program

program: main.o DynamicArray.o LinkedList.o Sequence.o
	g++ -o program main.o DynamicArray.o LinkedList.o Sequence.o

tests: Run_Tests.o Tests.o DynamicArray.o LinkedList.o Sequence.o
	g++ -o tests Run_Tests.o Tests.o DynamicArray.o LinkedList.o Sequence.o

main.o: main.cpp Sequence.hpp DynamicArray.hpp LinkedList.hpp
	g++ -c main.cpp

DynamicArray.o: DynamicArray.cpp DynamicArray.hpp LinkedList.hpp
	g++ -c DynamicArray.cpp

LinkedList.o: LinkedList.cpp LinkedList.hpp
	g++ -c LinkedList.cpp

Sequence.o: Sequence.cpp Sequence.hpp DynamicArray.hpp LinkedList.hpp
	g++ -c Sequence.cpp

run: program
	./program

test: tests.exe
	./tests.exe
	
clean:
	rm -f main.o DynamicArray.o LinkedList.o Sequence.o program