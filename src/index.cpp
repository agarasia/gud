// index.cpp
// Implementing reading/writing the .gud/index file

#include "index.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

const std::string INDEX_PATH = ".gud/index";

// Load the index into a map from the filepath -> blob hash
IndexMap loadIndex()
{
    IndexMap index;
    std::ifstream file(INDEX_PATH);
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string hash, path;
        if (iss >> hash >> std::ws && std::getline(iss, path))
        {
            index[path] = hash;
        }
    }

    return index;
}

// Writing the index to disk
void saveIndex(const IndexMap &index)
{
    std::ofstream file(INDEX_PATH, std::ios::trunc);
    for (const auto &[path, hash] : index)
    {
        file << hash << " " << path << "\n";
    }
}

// Update an in-memory index with a new file path and its blob hash
void updateIndex(IndexMap &index, const std::string &path, const std::string &hash)
{
    index[path] = hash;
}