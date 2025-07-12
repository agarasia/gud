#pragma once
#include <string>
#include <set>

std::string readFile(const std::string &path);
std::string findRootDir();
std::set<std::string> loadIgnoreList(const std::string &gudRoot);