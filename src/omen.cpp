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
#include "commit.hpp"
#include "utils.hpp"

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

// Helper function to load map<filepath, hash> from the latest committed tree object
IndexMap loadTree(const std::string &treeHash)
{
    IndexMap tree;
    std::string treeObjPath = ".gud/objects/" + treeHash.substr(0, 2) + "/" + treeHash.substr(2);
    std::ifstream treeFile(treeObjPath);
    if (!treeFile)
    {
        std::cerr << "\033[1;31m[OMEN]\033[0m Failed to read tree object: " << treeObjPath << "\n";
        return tree;
    }

    std::string line;
    while (std::getline(treeFile, line))
    {
        if (line.rfind("blob", 0) == 0)
        {
            std::string hashAndPath = line.substr(4);
            std::string hash = hashAndPath.substr(0, 40);
            std::string path = hashAndPath.substr(41);
            tree[path] = hash;
        }
    }

    return tree;
}

// Comparing Staged files with committed files
void compareStagingWithCommit(const IndexMap &index, const IndexMap &tree)
{
    std::set<std::string> printed;
    for (const auto &[path, hash] : index)
    {
        if (tree.count(path) == 0)
        {
            std::cout << "\033[1;32mStaged:\033[0m " << path << "\n";
            printed.insert(path);
        }
        else if (tree.at(path) != hash)
        {
            std::cout << "\033[1;33mModified in staging:\033[0m " << path << "\n";
            printed.insert(path);
        }
    }

    for (const auto &[path, hash] : tree)
    {
        if (printed.count(path) == 0)
        {
            std::cout << "\033[1;31mCommitted:\033[0m " << path << "\n";
        }
    }
}

// Comparing Working files with Staged files
void compareWorkingWithIndex(const IndexMap &index)
{
    std::set<std::string> seen;

    for (const auto &[path, stagedHash] : index)
    {
        seen.insert(path);
        if (!std::filesystem::exists(path))
        {
            std::cout << "\033[1;31mDeleted in working:\033[0m " << path << "\n";
        }
        else
        {
            std::string currentHash = computeFileHash(path);
            if (currentHash != stagedHash)
            {
                std::cout << "\033[1;33mModified in working:\033[0m " << path << "\n";
            }
        }
    }

    // Also scan for untracked files in the working directory
    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        std::string filename = entry.path().filename().string();
        if (entry.is_regular_file() &&
            filename != ".gud" &&
            filename != "main.cpp" &&
            filename != "Makefile" &&
            filename != ".gitignore" &&
            filename != "README.md" &&
            filename != "image.png" &&
            filename != ".DS_Store" &&
            entry.path().string().rfind(".gud", 0) == std::string::npos &&
            index.find(filename) == index.end())
            std::cout << "\033[1;32mUntracked in working:\033[0m " << filename << "\n";
    }
}

void handleOmen()
{
    std::cout << "\033[1;36m[OMEN]\033[0m Summoning the spirits of the file system...\n";

    std::string commitHash = getCurrentCommitHash();
    if (commitHash.empty())
    {
        std::cout << "\033[1;31m[OMEN]\033[0m The void is still. No commits have been made.\n";
        return;
    }

    // Load commit object content
    std::string commitObjPath = ".gud/objects/" + commitHash.substr(0, 2) + "/" + commitHash.substr(2);
    std::string commitContent = readFile(commitObjPath);

    std::istringstream iss(commitContent);
    std::string line;
    std::string treeHash;

    while (std::getline(iss, line))
    {
        if (line.rfind("tree", 0) == 0) // Corrected rfind usage
        {
            treeHash = line.substr(5);
            break;
        }
    }

    if (treeHash.empty())
    {
        std::cout << "\033[1;31m[OMEN]\033[0m No tree found in commit " << commitHash << ".\n";
        return;
    }

    auto committedFiles = loadTree(treeHash);
    auto stagedFiles = loadIndex();

    // Collect all working files except .gud and common build files
    std::set<std::string> workingFiles;
    for (const auto &entry : std::filesystem::directory_iterator("."))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string();

            // Skip .gud directory contents
            if (filename == ".gud")
                continue;

            // Skip build artifacts: *.o files and executable gud (adjust if your exe name differs)
            if (filename.size() > 2 && filename.substr(filename.size() - 2) == ".o")
                continue;

            if (filename == "gud")
                continue;

            workingFiles.insert(filename);
        }
    }

    // Remove files that are committed or staged from workingFiles to get untracked
    for (const auto &[path, _] : committedFiles)
    {
        workingFiles.erase(path);
    }
    for (const auto &[path, _] : stagedFiles)
    {
        workingFiles.erase(path);
    }

    // Report committed files that differ from working files
    for (const auto &[path, committedHash] : committedFiles)
    {
        // Compute current hash
        std::string currentHash = computeFileHash(path);

        if (currentHash.empty())
        {
            std::cout << "\033[1;31mDeleted:\033[0m " << path << "\n";
        }
        else if (currentHash != committedHash)
        {
            std::cout << "\033[1;33mModified:\033[0m " << path << "\n";
        }
        else
        {
            std::cout << "\033[1;32mCommitted:\033[0m " << path << "\n";
        }
    }

    // Report staged files that are new or modified compared to committed files
    for (const auto &[path, stagedHash] : stagedFiles)
    {
        if (committedFiles.find(path) == committedFiles.end())
        {
            std::cout << "\033[1;34mStaged (new):\033[0m " << path << "\n";
        }
        else if (committedFiles.at(path) != stagedHash)
        {
            std::cout << "\033[1;34mStaged (modified):\033[0m " << path << "\n";
        }
    }

    // Report untracked files (leftover in workingFiles set)
    for (const std::string &file : workingFiles)
    {
        std::cout << "\033[1;35mUntracked:\033[0m " << file << "\n";
    }

    std::cout << "\033[1;36m[OMEN]\033[0m The spirits have spoken. The omens of the realm is revealed.\n";
}
