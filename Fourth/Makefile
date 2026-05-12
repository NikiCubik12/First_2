CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

PROGRAM_SRCS := main.cpp
TESTS_SRCS   := Run_Tests.cpp

PROGRAM := program
TESTS   := tests.exe

HEADERS := $(wildcard *.hpp *.tpp)

.PHONY: all tests run clean

all: $(PROGRAM) $(TESTS)

$(PROGRAM): $(PROGRAM_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(PROGRAM_SRCS) -o $(PROGRAM)

$(TESTS): $(TESTS_SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(TESTS_SRCS) -o $(TESTS)

tests: $(TESTS)
	./$(TESTS)

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) $(TESTS)