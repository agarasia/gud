#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <iostream>

// Helper: Run a shell command and capture its stdout output as a string
std::string execAndCapture(const char *cmd)
{
    std::array<char, 128> buffer{};
    std::string result;
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        return "";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}

// Cleanup test artifacts
void cleanup()
{
    std::filesystem::remove_all(".gud");
    std::filesystem::remove("file1.txt");
    std::filesystem::remove("file2.txt");
}

// Test: create repo, add files, commit, log, check output
void testLogOutput()
{
    cleanup();

    // Initialize repository
    assert(system("./gud create") == 0);

    // Create test files
    {
        std::ofstream("file1.txt") << "First file content\n";
        std::ofstream("file2.txt") << "Second file content\n";
    }

    // Add files
    assert(system("./gud add file1.txt file2.txt") == 0);

    // Commit files with specific message
    const char *commitMsg = "My test commit";
    std::string commitCmd = std::string("./gud commit -m \"") + commitMsg + "\"";
    assert(system(commitCmd.c_str()) == 0);

    // Run log and capture output
    std::string output = execAndCapture("./gud log");

    // Debug print (optional)
    std::cout << "=== gud log output ===\n"
              << output << "=== end log output ===\n";

    // Assert commit message is in the output
    assert(output.find(commitMsg) != std::string::npos);

    std::cout << "[TEST] testLogOutput passed.\n";

    cleanup();
}

int main()
{
    testLogOutput();
    std::cout << "[TEST] All gud log tests passed!\n";
    return 0;
}
