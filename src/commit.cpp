// src/commit.cpp
// Implements 'gud commit -m "message"' by reading
// the index and writing a commit object.

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <ctime>
#include "index.hpp"
#include "hash.hpp"
#include "sha1.hpp"

std::string writeObject(const std::string &content)
{
    std::string hash = SHA1::hashString(content);
    std::string dir = ".gud/objects/" + hash.substr(0, 2);
    std::string file = hash.substr(2);
    std::filesystem::create_directories(dir);

    std::string path = dir + "/" + file;
    if (!std::filesystem::exists(path))
    {
        std::ofstream out(path);
        out << content;
    }
    return hash;
}

// Read HEAD and get the current parent commit hash (if any)
std::string getParentHash()
{
    std::ifstream head(".gud/HEAD");
    std::string ref;
    if (!std::getline(head, ref))
        return "";

    const std::string prefix = "ref: ";
    if (ref.rfind(prefix, 0) != 0)
    {
        std::cerr << "[Commit] Invalid HEAD reference.\n";
        return "";
    }

    std::string refPath = ".gud/" + ref.substr(prefix.size());
    std::ifstream branch(refPath);
    if (!branch)
        return "";

    std::string parent;
    std::getline(branch, parent);
    return parent;
}

// Write Tree Object and return its hash
std::string writeTree(const IndexMap &index)
{
    std::ostringstream oss;
    for (const auto &[path, hash] : index)
        oss << "blob" << hash << " " << path << "\n";

    return writeObject(oss.str());
}

std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[128];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

// A function to get the Current Commit Hash
std::string getCurrentCommitHash()
{
    std::ifstream head(".gud/HEAD");
    if (!head)
        return "";

    std::string refPath;
    std::getline(head, refPath);
    head.close();

    std::ifstream refFile(".gud/" + refPath);
    if (!refFile)
        return "";

    std::string currentHash;
    std::getline(refFile, currentHash);
    return currentHash;
}

// Write commit object with message and metadata
std::string writeCommit(const std::string &treeHash, const std::string &parentHash, const std::string &message)
{
    if (treeHash.empty())
    {
        std::cerr << "[Commit] Failed to write tree object.\n";
        return "";
    }

    std::ostringstream oss;
    oss << "tree " << treeHash << "\n";

    if (!parentHash.empty())
    {
        oss << "parent " << parentHash << "\n";
    }

    oss << "Author: Aum, Time: " << getCurrentTime() << "\n";
    oss << "Commit: " << message << "\n";

    std::string parentCommitHash = getCurrentCommitHash();
    if (!parentCommitHash.empty())
        oss << "Parent" << parentCommitHash << "\n";

    return writeObject(oss.str());
}

void handleCommit(const std::vector<std::string> &args)
{
    if (args.size() < 2 || args[0] != "-m")
    {
        std::cerr << "[COMMIT]  The abyss rejects thy command.\n"
                  << "          Here liest the proper incantation:\n"
                  << "          Usage: gud commit -m \"thy commit message\"\n";
        return;
    }

    std::string message = args[1];
    IndexMap index = loadIndex();

    if (index.empty())
    {
        std::cerr << "[COMMIT]  The void is empty, no changes to commit.\n";
        return;
    }

    std::string parentHash = getParentHash();
    std::string treeHash = writeTree(index);
    std::string commitHash = writeCommit(treeHash, parentHash, message);

    std::cout << "[Commit] A new commit hath been forged in fire.\n"
              << "         Hash: " << commitHash << "\n";

    // ✅ Clear index
    saveIndex({});

    // Update branch reference
    std::ifstream head(".gud/HEAD");
    std::string refLine;
    if (!std::getline(head, refLine) || refLine.rfind("ref: ", 0) != 0)
    {
        std::cerr << "[Commit] Invalid HEAD file format.\n";
        return;
    }

    std::string branchRef = refLine.substr(5); // safe now
    std::string branchPath = ".gud/" + branchRef;

    // Write commit hash to branch ref
    std::ofstream out(branchPath);
    if (!out)
    {
        std::cerr << "[Commit] Failed to update branch ref: " << branchPath << "\n";
        return;
    }
    out << commitHash << "\n";
}