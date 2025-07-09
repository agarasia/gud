# Makefile
# Build system for the gud version control tool written in C++

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Collect source files and object files
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = gud

# Default build target
all: $(BIN)

# Link the final binary from main.o and all source objects
$(BIN): main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ main.o $(OBJ)

# Compile the main.cpp file
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Generic rule to compile .cpp files in src/ to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up all compiled files and the binary
clean:
	rm -f *.o src/*.o $(BIN)
