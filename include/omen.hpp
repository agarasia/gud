// include/omen.hpp
// gud omen -> git status

#pragma once
#include <map>

void handleOmen();
std::string resolveHeadRef();                                             // Resolve the HEAD reference to get the current branch or commit
std::string getCurrentCommitHash();                                       // Get the current commit hash from the HEAD reference
std::map<std::string, std::string> loadTree(const std::string &treeHash); // Load the tree structure from a given hash