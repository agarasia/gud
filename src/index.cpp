// index.cpp
// Implementing reading/writing the .gud/index file

#include "index.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

const std::string INDEX_PATH = ".gud/index";

// Load the index into a map from the filepath -> blob hash
IndexMap loadIndex(const std::string &gudRoot)
{
    IndexMap index;
    std::filesystem::path idxPath = std::filesystem::path(gudRoot) / ".gud" / "index";
    std::ifstream file(idxPath);
    if (!file)
    {
        std::cerr << "\033[1;31m[ERROR]\033[0m Failed to open index file: " << gudRoot << INDEX_PATH << "\n";
        return index;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string hash, path;
        if (iss >> hash >> path)
        {
            index[path] = hash;
        }
    }

    return index;
}

// Writing the index to disk
void saveIndex(const IndexMap &index, const std::string &gudRoot)
{
    std::filesystem::path idxPath = std::filesystem::path(gudRoot) / ".gud" / "index";
    std::ofstream indexFile(idxPath, std::ios::trunc);

    if (!indexFile)
    {
        std::cerr << "\033[1;31m[ERROR]\033[0m Failed to open index file: " << idxPath << "\n";
        return;
    }

    for (const auto &[path, hash] : index)
    {
        indexFile << hash << " " << path << "\n";
    }
}

// Update an in-memory index with a new file path and its blob hash
void updateIndex(IndexMap &index, const std::string &path, const std::string &hash)
{
    index[path] = hash;
}