# gud - A version control system in C++
# Makefile to build the gud CLI tool

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Source files
SRC = main.cpp \
      src/create.cpp \
      src/add.cpp \
      src/commit.cpp \
      src/hash.cpp \
      src/index.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output binary
BIN = gud

# Default build target
all: $(BIN)

# Build executable
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Compile each .cpp file to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean