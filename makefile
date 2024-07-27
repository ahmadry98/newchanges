# Makefile for Software-Project-hw2

# Compiler options
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic

# Target executable
TARGET = myrobot

# Source files
SRCS = main.cpp Hoover.cpp House.cpp Sensors.cpp CleaningAlgorithm.cpp MySimulator.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Dependency files
DEPS = $(SRCS:.cpp:.d)

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -mconsole

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Include dependencies
-include $(DEPS)

# Generate dependencies
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MT '$(@:.d=.o)' -MM $< > $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

.PHONY: all clean
