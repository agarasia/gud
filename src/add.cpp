// Implementaion of the 'git add' command

#include <iostream>
#include <fstream>
#include <filesystem>
#include "hash.hpp"
#include "index.hpp"

void handleAdd(const std::vector<std::string> &args)
{
    // Steps:
    // 1. Read the file contents.
    // 2. Hash the contents to create a unique identifier.
    // 3. Store the hash in the .gud/objects/<first-two-hex>/<rest>.blob.
    // 4. Update the index file (I'll stub this for now).

    // We'll go with a header-only SHA-1 hash for simplicity.

    if (args.empty())
    {
        std::cerr << "[ADD] Ahh... Thou hast not provided a file to add.\n"
                  << "      The shadows mock thy empty hands, \n"
                  << "      For a vessel without contents is but a hollow shell.\n"
                  << "      Perchance thou seekest to add a file to the annals of gud?\n";
        std::cerr << "Usage: gud add <file>\n";
        return;
    }

    // Load the index to track staged files
    IndexMap index = loadIndex();

    for (const auto &path : args)
    {
        if (!std::filesystem::exists(path))
        {
            std::cerr << "[ADD] Alas, the file '" << path << "' doth not exist.\n"
                      << "      The shadows mock thy empty hands, \n"
                      << "      For a vessel without contents is but a hollow shell.\n"
                      << "      Perchance thou seekest to add a file to the annals of gud?\n";
            continue;
        }

        // Hash the file content
        std::string hash = sha1FromFile(path);
        if (hash.empty())
        {
            std::cerr << "[ADD] Alas, the shadows have swallowed the contents of '" << path << "'.\n"
                      << "      The abyss answers not, for such utterances are unknown.\n";
            continue;
        }

        // Prepare storage path: .gud/objects/<first-two-hex>/<rest>.blob
        std::string dir = ".gud/objects/" + hash.substr(0, 2);
        std::string file = hash.substr(2);
        std::filesystem::create_directories(dir);

        std::string fullPath = dir + "/" + file;

        // Avoiding rewrites for already stored files
        if (std::filesystem::exists(fullPath))
        {
            std::cout << "[Add] Failed to hash file: " << path << "\n"
                      << "      The shadows mock thy empty hands, \n"
                      << "      For a vessel without contents is but a hollow shell.\n"
                      << "      Perchance thou seekest to add a file to the annals of gud?\n";
            continue;
        }

        // Read and Write the file content to the blob
        std::ifstream in(path, std::ios::binary);
        std::ofstream out(fullPath, std::ios::binary);
        out << in.rdbuf();

        std::cout << "[ADD] The file '" << path << "' hath been added to the annals of gud.\n"
                  << "      Its essence now resides within the shadows, \n"
                  << "      A testament to thy deeds, forever etched in time.\n";

        // Update the index with the new file path and its hash
        updateIndex(index, path, hash);
    }

    // Save the updated index
    saveIndex(index);
}