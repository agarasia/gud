#pragma once
#ifndef GUD_UTILS_HPP
#define GUD_UTILS_HPP
#include <string>
#include <set>
#include <vector>

std::string readFile(const std::string &path);
std::string findGudRoot();
std::set<std::string> loadIgnoreList(const std::string &gudRoot);
bool isIgnored(const std::string &path, const std::set<std::string> &ignoreList);
#endif