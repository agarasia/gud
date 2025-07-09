// src/hash.cpp
// Wrapper around header-only SHA-1 class

#include "hash.hpp"
#include "sha1.hpp"

std::string sha1FromFile(const std::string &path)
{
    return SHA1::hashFile(path);
}
