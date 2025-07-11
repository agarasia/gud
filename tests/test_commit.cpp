#include <cassert>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/index.hpp"
#include "../include/hash.hpp"

void cleanupRepo()
{
    std::filesystem::remove_all(".gud");
    std::filesystem::remove("a.txt");
    std::filesystem::remove("b.txt");
    std::filesystem::remove("c.txt");
}

void testMultipleFileCommit()
{
    cleanupRepo();

    system("./gud create");

    // Create multiple files
    {
        std::ofstream("a.txt") << "Seek strength.";
        std::ofstream("b.txt") << "Let strength be granted...";
        std::ofstream("c.txt") << "So the world might be mended.";
    }

    // Add all files
    system("./gud add a.txt b.txt c.txt");

    // Commit them
    int result = system("./gud commit -m \"Three blessings\"");
    assert(result == 0);

    // Read HEAD reference
    std::ifstream head(".gud/HEAD");
    std::string headRef;
    std::getline(head, headRef);
    assert(headRef == "ref: refs/heads/master");

    // Read commit hash
    std::ifstream ref(".gud/refs/heads/master");
    std::string commitHash;
    std::getline(ref, commitHash);
    assert(!commitHash.empty());

    std::string commitPath = ".gud/objects/" + commitHash.substr(0, 2) + "/" + commitHash.substr(2);
    assert(std::filesystem::exists(commitPath));

    // Check index is now cleared
    IndexMap index = loadIndex();
    assert(index.empty());

    std::cout << "[TEST] testMultipleFileCommit passed.\n";

    cleanupRepo();
}

int main()
{
    testMultipleFileCommit();
    std::cout << "[TEST] All multiple-file commit tests passed!\n";
    return 0;
}
