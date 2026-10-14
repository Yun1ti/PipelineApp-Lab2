#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ConsoleMenu.h"
#include "InputReader.h"

std::string MakeLogFileName()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::ostringstream fileName;
    fileName << "log_" << std::put_time(&localTime, "%Y%m%d_%H%M%S") << ".txt";
    return fileName.str();
}

int main()
{
    std::string logFileName = MakeLogFileName();
    InputReader reader(std::cin, logFileName);
    if (reader.IsLogging())
        std::cout << "User input is logged to " << logFileName << "\n";
    else
        std::cout << "Warning: cannot create log file " << logFileName << "\n";

    ConsoleMenu menu(reader);
    menu.Run();
    return 0;
}
