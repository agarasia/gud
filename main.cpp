// Entry point for 'gud' version control system
// Handles parsing of CLI arguments and dispatches
// to appropriate commands

#include <iostream>
#include <string>
#include <vector>
#include "log.hpp"
#include "omen.hpp"

void handleCreate();
void handleAdd(const std::vector<std::string> &args);
void handleCommit(const std::vector<std::string> &args);

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string cmd = argv[1];
        if (cmd == "log")
        {
            logCommits();
            return 0;
        }
        else if (cmd == "omen")
        {
            omen();
            return 0;
        }
    }

    // Ensure we have at least one argument
    if (argc < 2)
    {
        std::cerr << "Usage: gud <command> [options]\n";
        return 1;
    }

    // Extract the command from the arguments
    std::string command = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    // Route the command to the appropriate handler
    if (command == "create")
    {
        handleCreate();
    }
    else if (command == "add")
    {
        handleAdd(args);
    }
    else if (command == "commit")
    {
        handleCommit(args);
    }
    else
    {
        // Handle unknown command (in an oldern English style)
        std::cerr << "Ahh... Thou utterest words lost to this forsaken land. \n"
                  << "The echoes of thy command fade into the void, \n"
                  << "And the shadows mock thy ignorance.\n"
                  << "Perchance thou seekest to kindle a new repository, \n"
                  << "Or add thy deeds to the annals of gud?\n"
                  << "Or mayhap thou wishest to commit thy actions to memory?\n\n";
        std::cerr << "Alas, the abyss answers not, for such utterances are unknown.\n"
                  << "Shouldst thou seek guidance, ponder these paths:\n\n"
                  << "create\n"
                  << "add\n"
                  << "commit\n\n"
                  << "Choose wisely, for not all utterances find purchase in the darkness.\n";
        return 1;
    }

    return 0;
}