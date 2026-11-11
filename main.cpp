#include <fstream>
#include <iostream>
#include <string>

#include "ConsoleMenu.h"
#include "InputReader.h"

const std::string LOG_FILE_NAME = "log.txt";

// Usage:
//   PipelineApp.exe            - interactive mode, user input is written to log.txt
//   PipelineApp.exe log.txt    - replay mode, actions are read from the given file
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::ifstream replayFile(argv[1]);
        if (!replayFile)
        {
            std::cout << "Error: cannot open file " << argv[1] << "\n";
            return 1;
        }
        std::cout << "Replaying actions from " << argv[1] << "\n";
        InputReader reader(replayFile, true);
        ConsoleMenu(reader).Run();
        return 0;
    }

    InputReader reader(std::cin, false);
    if (reader.StartLogging(LOG_FILE_NAME))
        std::cout << "User input is logged to " << LOG_FILE_NAME << "\n";
    else
        std::cout << "Warning: cannot create log file " << LOG_FILE_NAME << "\n";
    ConsoleMenu(reader).Run();
    return 0;
}
