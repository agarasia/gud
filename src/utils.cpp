#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "utils.hpp"

std::string readFile(const std::string &path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
        return "";
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string findGudRoot()
{
    std::filesystem::path path = std::filesystem::current_path();
    while (!path.empty())
    {
        if (std::filesystem::exists(path / ".gud"))
            return path.string();
        path = path.parent_path();
    }
    return ""; // Not in a gud repo
}

std::set<std::string> loadIgnoreList(const std::string &gudRoot)
{
    std::set<std::string> ignore;
    std::ifstream in(gudRoot + "/.gudignore");
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
            ignore.insert(line);
    }

    // Hardcoded ignores
    ignore.insert("gud");
    ignore.insert("main.o");
    ignore.insert("a.out");

    return ignore;
}