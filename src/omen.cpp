// src/omen.cpp
// Implements 'gud omen' - a command to
// display the current git status.

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <set>
#include "hash.hpp"
#include "index.hpp"
#include "sha1.hpp"

namespace fs = std::filesystem;

std::string computeFileHash(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return "";

    std::ostringstream ss;
    ss << file.rdbuf();
    return SHA1::hashString(ss.str());
}

void omen()
{
    IndexMap index = loadIndex();
    std::set<std::string> workingFiles;

    for (const auto &entry : fs::directory_iterator("."))
    {
        if (entry.is_regular_file() && entry.path().string().rfind(".gud", 0) == std::string::npos)
        {
            workingFiles.insert(entry.path().string().substr(2)); // Strip ./ prefix
        }
    }

    std::cout << "\033[1;36m[OMEN]\033[0m Reading signs of the file system...\n";

    bool changesFound = false;

    for (const auto &[path, hash] : index)
    {
        std::string currentHash = computeFileHash(path);
        if (currentHash.empty())
        {
            std::cout << "\033[1;31mDeleted:\033[0m " << path << "\n";
            changesFound = true;
        }
        else if (currentHash != hash)
        {
            std::cout << "\033[1;33mModified:\033[0m " << path << "\n";
            changesFound = true;
        }
        workingFiles.erase(path);
    }

    for (const std::string &file : workingFiles)
    {
        std::cout << "\033[1;32mUntracked:\033[0m " << file << "\n";
        changesFound = true;
    }

    if (!changesFound)
    {
        std::cout << "\033[1;32m[OMEN]\033[0m The realm is calm. No foul presences detected.\n";
    }
}