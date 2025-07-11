// src/log.cpp
// Implementing 'gud log' to display commit history

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <map>

// ANSI color codes for terminal output
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
#define COLOR_RED "\033[31m"
#define COLOR_BOLD "\033[1m"

// Helper function to read whole file into a string
std::string readFile(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
        return "";
    std::ostringstream oss;
    oss << in.rdbuf();
    return oss.str();
}

// Helper function to lowercase a string
std::string toLower(const std::string &str)
{
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return lower;
}

// Parse commit file key: value pairs into a map
std::map<std::string, std::string> parseCommit(const std::string &content)
{
    std::map<std::string, std::string> fields;
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty())
            continue;

        std::string key, value;
        auto colonPos = line.find(':');
        auto spacePos = line.find(' ');

        if (colonPos != std::string::npos && (colonPos < spacePos || spacePos == std::string::npos))
        {
            // Format: Key: Value
            key = line.substr(0, colonPos);
            value = line.substr(colonPos + 1);
        }
        else if (spacePos != std::string::npos)
        {
            // Format: key value
            key = line.substr(0, spacePos);
            value = line.substr(spacePos + 1);
        }
        else
        {
            continue;
        }

        // Trim whitespace
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n"));

        // Normalize key to lowercase
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        fields[key] = value;
    }

    return fields;
}

// Display commit information
void logCommits()
{
    namespace fs = std::filesystem;
    const std::string headPath = ".gud/HEAD";

    // Read HEAD to find current branch ref
    std::ifstream headFile(headPath);
    if (!headFile.is_open())
    {
        std::cerr << "[log] Could not open HEAD file. Is this a gud repository?\n";
        return;
    }

    std::string headRef;
    std::getline(headFile, headRef);
    headFile.close();

    // Handle "ref: refs/heads/master" format
    if (headRef.rfind("ref: ", 0) == 0)
    {
        headRef = headRef.substr(5); // strip "ref: "
    }

    if (headRef.empty())
    {
        std::cerr << "[log] HEAD is empty. No commits to show.\n";
        return;
    }

    // Compose full path to ref file
    std::string refPath = ".gud/" + headRef;
    std::ifstream refFile(refPath);
    if (!refFile.is_open())
    {
        std::cerr << "[log] Could not open ref file " << refPath << "\n";
        return;
    }

    // Read commit hash from ref
    std::string commitHash;
    std::getline(refFile, commitHash);
    refFile.close();

    if (commitHash.empty())
    {
        std::cerr << "[log] No commits found on branch.\n";
        return;
    }

    // Read the commit object file
    std::string commitPath = ".gud/objects/" + commitHash.substr(0, 2) + "/" + commitHash.substr(2);
    std::ifstream commitFile(commitPath);
    if (!commitFile.is_open())
    {
        std::cerr << "[log] Could not open commit object " << commitPath << "\n";
        return;
    }

    std::stringstream buffer;
    buffer << commitFile.rdbuf();
    commitFile.close();

    auto fields = parseCommit(buffer.str());

    std::cout << "\033[1;34mCommit: " << commitHash << "\n";

    if (fields.count("tree"))
    {
        std::cout << "\033[1;32mTree:   " << fields["tree"] << "\n";
    }

    if (fields.count("author"))
    {
        std::cout << "\033[1;36mAuthor: " << fields["author"] << "\n";
    }

    if (fields.count("commit"))
    {
        std::cout << "\033[1;33mMessage: " << fields["commit"] << "\n";
    }

    std::cout << "\n";
}