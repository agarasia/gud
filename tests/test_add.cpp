#include <cassert>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/index.hpp"
#include "../include/hash.hpp"

void cleanupRepo()
{
    std::filesystem::remove_all(".gud");
    std::filesystem::remove("test.txt");
}

void testAddSingleFile()
{
    cleanupRepo();

    system("./gud create");

    // Create test file
    {
        std::ofstream out("test.txt");
        out << "Hello, Soulsborne!";
    }

    // Run `gud add`
    int result = system("./gud add test.txt");
    assert(result == 0);

    // Load index
    IndexMap index = loadIndex();
    assert(index.find("test.txt") != index.end());

    // Verify object was written
    std::string hash = index["test.txt"];
    std::string objPath = ".gud/objects/" + hash.substr(0, 2) + "/" + hash.substr(2);
    assert(std::filesystem::exists(objPath));

    std::cout << "[TEST] testAddSingleFile passed.\n";

    cleanupRepo();
}

int main()
{
    testAddSingleFile();
    std::cout << "[TEST] All gud add tests passed!\n";
    return 0;
}
