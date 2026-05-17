test:
	g++ Run_Tests.cpp Sequence.cpp BitSequence.cpp -o tests.exe -std=c++17 -Wall -g -I.
	./tests.exe

server:
	g++ server.cpp Sequence.cpp BitSequence.cpp -o server.exe -std=c++17 -Wall -g -I. -lws2_32

run:
	./server.exe

start:
	./server.exe

clean:
	rm -f tests.exe server.exe *.o

.PHONY: test server run start clean