// src/log.cpp
// Implementing 'gud log' to display commit history

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <map>
#include "utils.hpp"

// ANSI color codes for terminal output
#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"
#define COLOR_RED "\033[31m"
#define COLOR_BOLD "\033[1m"

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

// Helper function to get the correct reference path from HEAD
std::string resolveHeadRef()
{
    std::ifstream headFile(".gud/HEAD");
    std::string line;
    if (!headFile || !std::getline(headFile, line))
        return "";

    if (line.rfind("ref: ", 0) == 0)
    {
        return ".gud/" + line.substr(5);
    }
    return ".gud/" + line; // Direct commit hash
}

// Display commit information
void logCommits()
{
    std::ifstream headFile(".gud/HEAD");
    if (!headFile)
    {
        std::cerr << COLOR_RED << "[LOG] No HEAD found. Are you in a gud repository?" << COLOR_RESET << std::endl;
        return;
    }

    std::string refPath = resolveHeadRef();
    std::ifstream refFile(refPath);
    if (!refFile)
    {
        std::cerr << COLOR_RED << "[LOG]    The annals yield only silence,\n"
                  << "         Unkindled—the reference file "
                  << refPath << "\n"
                  << "         is lost to the abyss" << COLOR_RESET << std::endl;
        return;
    }

    std::string currentHash;
    std::getline(refFile, currentHash);

    std::cout << COLOR_CYAN << "[LOG]    The annals of Gud, a chronicle of thy deeds:" << COLOR_RESET << std::endl;

    while (!currentHash.empty())
    {
        std::string objPath = ".gud/objects/" + currentHash.substr(0, 2) + "/" + currentHash.substr(2);
        std::string content = readFile(objPath);
        if (content.empty())
            break;

        auto fields = parseCommit(content);

        std::cout << COLOR_BOLD << COLOR_GREEN << "Commit: " << currentHash << COLOR_RESET << std::endl;
        std::cout << COLOR_YELLOW << "Author: " << fields["author"] << COLOR_RESET << std::endl;
        std::cout << COLOR_YELLOW << "Message: " << fields["commit"] << COLOR_RESET << std::endl;
        std::cout << COLOR_CYAN << "Tree: " << fields["tree"] << COLOR_RESET << std::endl;
        std::cout << COLOR_CYAN << "Parent: " << (fields.count("parent") ? fields["parent"] : "None") << COLOR_RESET << std::endl;
        std::cout << COLOR_CYAN << "----------------------------------------" << COLOR_RESET << std::endl;
        std::cout << "\n";

        // Traverse to parent commit
        if (fields.count("parent"))
            currentHash = fields["parent"];
        else
            break;
    }

    std::cout << COLOR_CYAN << "[LOG]    The annals have been read, and the tale is told." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    May thy journey through Gud be ever fruitful." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    Remember, the past is but a guide, not a chain." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    Farewell, brave traveler." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    The annals await thy next chapter." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    Until we meet again, in the realm of Gud." << COLOR_RESET << std::endl;
    std::cout << COLOR_CYAN << "[LOG]    May the light of Gud guide thy path." << COLOR_RESET << std::endl;
}
