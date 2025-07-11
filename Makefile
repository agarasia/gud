# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# === Source Files ===
SRC = src/create.cpp src/add.cpp src/commit.cpp src/index.cpp src/hash.cpp src/log.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

# === Executable ===
BIN = gud

# === Test Binaries ===
TESTS = tests/test_add tests/test_commit tests/test_commit_multiple tests/test_log

# === Default Target ===
all: $(BIN)

# === Build main binary ===
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Build test binaries ===
tests/test_add: tests/test_add.cpp src/index.cpp src/hash.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tests/test_commit: tests/test_commit.cpp src/index.cpp src/commit.cpp src/hash.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tests/test_commit_multiple: tests/test_commit_multiple.cpp src/index.cpp src/commit.cpp src/hash.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tests/test_log: tests/test_log.cpp src/index.cpp src/log.cpp src/hash.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Run tests ===
test: $(TESTS)
	@echo "🧪 Running gud add tests..."
	@./tests/test_add && echo "✅ gud add passed" || echo "❌ gud add failed"

	@echo "🧪 Running gud commit tests..."
	@./tests/test_commit && echo "✅ gud commit passed" || echo "❌ gud commit failed"

	@echo "🧪 Running gud commit multiple tests..."
	@./tests/test_commit_multiple && echo "✅ gud commit (multiple) passed" || echo "❌ gud commit (multiple) failed"

	@echo "🧪 Running gud log tests..."
	@./tests/test_log && echo "✅ gud log passed" || echo "❌ gud log failed"

# === Clean ===
clean:
	rm -f $(OBJ) $(BIN) $(TESTS) tests/*.o
	rm -rf .gud
	rm -f *.txt

.PHONY: all clean test
