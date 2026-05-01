all: program.exe

program.exe: main.o
	g++ -o program.exe main.o

tests.exe: Run_Tests.o
	g++ -o tests.exe Run_Tests.o

main.o: main.cpp Sequence.hpp DynamicArray.hpp LinkedList.hpp
	g++ -c main.cpp

Run_Tests.o: Run_Tests.cpp Tests.hpp Sequence.hpp DynamicArray.hpp LinkedList.hpp
	g++ -c Run_Tests.cpp

run: program.exe
	./program.exe

test: tests.exe
	./tests.exe

clean:
	rm -f *.o *.exe