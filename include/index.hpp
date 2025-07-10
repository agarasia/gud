// Index.hpp
// Manages .gud/index for tracking staged files

#pragma once
#include <string>
#include <unordered_map>

// Key: file path, Value: blob hash
using IndexMap = std::unordered_map<std::string, std::string>;

IndexMap loadIndex();
void saveIndex(const IndexMap &index);
void updateIndex(IndexMap &index, const std::string &path, const std::string &hash);