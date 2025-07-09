// include/hash.hpp
// Wrapper for file hashing using header-only SHA-1

#pragma once
#include <string>

std::string sha1FromFile(const std::string &path);
