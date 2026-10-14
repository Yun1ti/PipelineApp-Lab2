#include "StreamUtils.h"

bool ContainsOnlySpaces(const std::string& text)
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
