// Implementation of the 'git add' command

#include <iostream>
#include <fstream>
#include <filesystem>
#include "hash.hpp"
#include "index.hpp"

void handleAdd(const std::vector<std::string> &args)
{
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

        // Prepare storage path: .gud/objects/<first-two-hex>/<rest> (no extension)
        std::string dir = ".gud/objects/" + hash.substr(0, 2);
        std::string file = hash.substr(2);
        std::filesystem::create_directories(dir);

        std::string fullPath = dir + "/" + file;

        // Write blob file only if it doesn't exist
        if (!std::filesystem::exists(fullPath))
        {
            std::ifstream in(path, std::ios::binary);
            std::ofstream out(fullPath, std::ios::binary);
            out << in.rdbuf();
            std::cout << "[ADD] The file '" << path << "' hath been added to the annals of gud.\n"
                      << "      Its essence now resides within the shadows, \n"
                      << "      A testament to thy deeds, forever etched in time.\n";
        }
        else
        {
            std::cout << "[ADD] The file '" << path << "' is already known to gud, no new shadow cast.\n";
        }

        // Always update index: path → hash
        updateIndex(index, path, hash);
    }

    // Save the updated index
    saveIndex(index);
}