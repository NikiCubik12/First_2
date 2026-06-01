CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -MMD -MP
ifeq ($(OS),Windows_NT)
	LDFLAGS = -lws2_32
	RUN_TARGET = ./$(TARGET).exe
	CLEAN_CMD = del /f $(OBJS) $(TARGET).exe 2>nul || rm -f $(OBJS) $(TARGET)
else
	LDFLAGS =
	RUN_TARGET = ./$(TARGET)
	CLEAN_CMD = rm -f $(OBJS) $(TARGET)
endif

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN_TARGET)

clean:
	$(CLEAN_CMD) $(DEPS)

.PHONY: all clean run

-include $(DEPS)
