#include "InputUtils.h"

static bool ContainsOnlySpaces(const std::string& text)
{
    return text.find_first_not_of(" \t") == std::string::npos;
}

bool ReadLineFromStream(std::istream& stream, std::string& line)
{
    if (!std::getline(stream, line))
        return false;
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    return true;
}

bool ReadNonEmptyLineFromStream(std::istream& stream, std::string& line)
{
    return ReadLineFromStream(stream, line) && !ContainsOnlySpaces(line);
}

std::string ReadLine()
{
    std::string line;
    if (!ReadLineFromStream(std::cin, line))
        throw InputClosedError();
    return line;
}

std::string ReadNonEmptyLine(const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        std::string line = ReadLine();
        if (!ContainsOnlySpaces(line))
            return line;
        std::cout << "Error: value cannot be empty.\n";
    }
}

bool ReadYesNo(const std::string& prompt)
{
    return ReadNumber(prompt + " (1 - yes, 0 - no): ", 0, 1) == 1;
}
