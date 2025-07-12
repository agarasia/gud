// inclued/commit.hpp
// Commit logic and object writing

#pragma once
#include <string>

void handleCommit(const std::vector<std::string> &args);
std::string getCurrentCommitHash(); // Get the current commit hash from the HEAD reference