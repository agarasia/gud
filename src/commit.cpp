// src/commit.cpp
// Stub implementation for the `gud commit` command.
// This will eventually write the staged files to a tree and create a commit object.

#include <iostream>
#include <vector>

void handleCommit(const std::vector<std::string> &args)
{
    std::cout << "[Commit] Not implemented yet. Args:\n";

    // Print the arguments passed to `gud commit` (e.g., commit message)
    for (const auto &arg : args)
    {
        std::cout << "  " << arg << "\n";
    }
}