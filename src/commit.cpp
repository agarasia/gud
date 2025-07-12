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
#include "utils.hpp"

std::string writeObject(const std::string &content, const std::string &gudRoot)
{
    std::string hash = SHA1::hashString(content);
    std::string dir = gudRoot + "/.gud/objects/" + hash.substr(0, 2);
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
std::string getParentHash(const std::string &gudRoot)
{
    std::ifstream head(gudRoot + "/.gud/HEAD");
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
std::string writeTree(const IndexMap &index, const std::string &gudRoot)
{
    std::ostringstream oss;
    for (const auto &[path, hash] : index)
        oss << "blob" << hash << " " << path << "\n";

    return writeObject(oss.str(), gudRoot);
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
std::string getCurrentCommitHash(const std::string &gudRoot)
{
    std::ifstream head(gudRoot + "/.gud/HEAD");
    if (!head)
        return "";

    std::string refLine;
    std::getline(head, refLine);
    head.close();

    if (refLine.rfind("ref: ", 0) != 0)
        return refLine;

    std::string refPath = gudRoot + "/.gud/" + refLine.substr(5);
    std::ifstream refFile(refPath);
    if (!refFile)
        return "";

    std::string currentHash;
    std::getline(refFile, currentHash);
    return currentHash;
}

// Write commit object with message and metadata
std::string writeCommit(const std::string &treeHash, const std::string &parentHash, const std::string &message, const std::string &gudRoot)
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

    std::string parentCommitHash = getCurrentCommitHash(gudRoot);
    if (!parentCommitHash.empty())
        oss << "Parent" << parentCommitHash << "\n";

    return writeObject(oss.str(), gudRoot);
}

void handleCommit(const std::vector<std::string> &args)
{
    std::string gudRoot = findGudRoot();
    if (gudRoot.empty())
    {
        std::cerr << "[Commit] No .gud repository found in this or parent directories.\n";
        return;
    }

    if (args.size() < 2 || args[0] != "-m")
    {
        std::cerr << "[COMMIT]  The abyss rejects thy command.\n"
                  << "          Here liest the proper incantation:\n"
                  << "          Usage: gud commit -m \"thy commit message\"\n";
        return;
    }

    std::string message = args[1];
    IndexMap index = loadIndex(gudRoot);

    if (index.empty())
    {
        std::cerr << "[COMMIT]  The void is empty, no changes to commit.\n";
        return;
    }

    std::string parentHash = getParentHash(gudRoot);
    std::string treeHash = writeTree(index, gudRoot);
    std::string commitHash = writeCommit(treeHash, parentHash, message, gudRoot);

    std::cout << "[Commit] A new commit hath been forged in fire.\n"
              << "         Hash: " << commitHash << "\n";

    // ✅ Clear index
    saveIndex({}, gudRoot);

    // Update branch reference
    std::ifstream head(gudRoot + "/.gud/HEAD");
    std::string refLine;
    if (!std::getline(head, refLine) || refLine.rfind("ref: ", 0) != 0)
    {
        std::cerr << "[Commit] Invalid HEAD file format.\n";
        return;
    }

    std::string branchRef = refLine.substr(5); // safe now
    std::string branchPath = gudRoot + "/.gud/" + branchRef;

    std::ofstream out(branchPath);
    if (!out)
    {
        std::cerr << "[Commit] Failed to update branch ref: " << branchPath << "\n";
        return;
    }
    out << commitHash << "\n";
}