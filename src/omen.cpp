// src/omen.cpp
// Implements 'gud omen' - a command to
// display the current git status.

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <set>
#include <sstream>
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

// Helper function to load map<filepath, hash> from the latest committed tree object
IndexMap loadTree(const std::string &treeHash, const std::string &gudRoot)
{
    IndexMap tree;
    std::string objPath = gudRoot + "/.gud/objects/" + treeHash.substr(0, 2) + "/" + treeHash.substr(2);
    std::ifstream file(objPath);
    if (!file)
    {
        std::cerr << "\033[1;31m[ERROR]\033[0m Failed to open tree object: " << objPath << "\n";
        return tree;
    }

    std::string line;
    while (std::getline(file, line))
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
void compareStagingWithCommit(const IndexMap &staged, const IndexMap &committed)
{
    std::set<std::string> printed;
    for (const auto &[path, hash] : staged)
    {
        if (committed.count(path) == 0)
        {
            std::cout << "\033[1;32mStaged:\033[0m " << path << "\n";
            printed.insert(path);
        }
        else if (committed.at(path) != hash)
        {
            std::cout << "\033[1;33mModified in staging:\033[0m " << path << "\n";
            printed.insert(path);
        }
    }

    for (const auto &[path, hash] : committed)
    {
        if (printed.count(path) == 0)
        {
            std::cout << "\033[1;31mCommitted:\033[0m " << path << "\n";
        }
    }
}

// Comparing Working files with Staged files and Committed files
void compareWorkingWithIndex(const IndexMap &staged,
                             const IndexMap &committed,
                             const std::string &gudRoot,
                             const std::set<std::string> &ignoreList)
{
    fs::current_path(gudRoot);

    // Track files that have been processed to avoid duplicates
    std::set<std::string> seen;

    for (const auto &[path, stagedHash] : staged)
    {
        seen.insert(path);
        if (!fs::exists(path))
        {
            std::cout << "\033[1;31mDeleted:\033[0m " << path << "\n";
        }
        else
        {
            std::string currentHash = computeFileHash(path);
            if (currentHash != stagedHash)
            {
                std::cout << "\033[1;33mModified:\033[0m " << path << "\n";
            }
        }
    }

    for (const auto &entry : fs::directory_iterator(gudRoot))
    {
        std::string filename = entry.path().filename().string();
        if (entry.is_regular_file() &&
            ignoreList.find(filename) == ignoreList.end() &&
            staged.find(filename) == staged.end() &&
            committed.find(filename) == committed.end())
        {
            std::cout << "\033[1;32mUntracked:\033[0m " << filename << "\n";
        }
    }
}

void handleOmen()
{
    std::string gudRoot = findGudRoot();
    if (gudRoot.empty())
    {
        std::cerr << "\033[1;31m[OMEN]\033[0m   The omens are dire, Unkindled - no .gud directory found\n"
                  << "Thou wanderest outside the bounds of a Gud repository. \n"
                  << "Seek the sacred .gud directory to commune with the spirits of Gud.\n";
        return;
    }

    std::cout << "\033[1;36m[OMEN]\033[0m   The omens are clear, Unkindled - you are in the realm of Gud.\n";
    std::cout << "\033[1;36m[OMEN]\033[0m   Reading the signs...\n";

    auto ignoreList = loadIgnoreList(gudRoot);
    std::string commitHash = getCurrentCommitHash(gudRoot);
    if (commitHash.empty())
    {
        std::cerr << "\033[1;31m[OMEN]\033[0m   The void is still. No commits have been made.\n";
        return;
    }

    std::string commitObjPath = gudRoot + "/.gud/objects/" + commitHash.substr(0, 2) + "/" + commitHash.substr(2);
    std::string commitContent = readFile(commitObjPath);

    std::istringstream iss(commitContent);
    std::string line, treeHash;

    while (std::getline(iss, line))
    {
        if (line.rfind("tree ", 0) == 0)
        {
            treeHash = line.substr(5);
            break;
        }
    }

    if (treeHash.empty())
    {
        std::cerr << "\033[1;31m[OMEN]\033[0m   The commit has no tree hash. Cannot proceed.\n";
        return;
    }

    auto committed = loadTree(treeHash, gudRoot);
    auto staged = loadIndex(gudRoot);

    compareStagingWithCommit(staged, committed);
    compareWorkingWithIndex(staged, committed, gudRoot, ignoreList);
}
