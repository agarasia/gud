// Handles the 'gud create' command

#include <iostream>
#include <fstream>
#include <filesystem>

void handleCreate()
{
    // Create necessary directories for the .gud repo
    std::filesystem::create_directories(".gud/objects");
    std::filesystem::create_directories(".gud/refs");
    std::filesystem::create_directories(".gud/refs/heads");

    // Create the HEAD file and point it to the master branch
    std::ofstream headFile(".gud/HEAD");
    headFile << "ref: refs/heads/master\n";

    // ✅ Create an empty master branch ref
    std::ofstream master(".gud/refs/heads/master");
    master << ""; // optional; or leave empty

    std::cout << "Ah, so thou hast kindled an empty repository of gud...\n"
              << "A repository, like a vessel, awaits its purpose.\n"
              << "Mayhap thou shalt fill it with thy noble deeds,\n"
              << "Or mayhap it shall remain a hollow shell.\n"
              << "Tread carefully, Unkindled.\n";
    std::cout << "\n";
}