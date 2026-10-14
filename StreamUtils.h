#pragma once

#include <istream>
#include <sstream>
#include <string>

bool ReadLineFromStream(std::istream& stream, std::string& line);
bool ReadNonEmptyLineFromStream(std::istream& stream, std::string& line);
bool ContainsOnlySpaces(const std::string& text);

template <typename T>
bool TryParseNumber(const std::string& text, T& result)
{
    std::istringstream stream(text);
    T value;
    if (!(stream >> value))
        return false;
    stream >> std::ws;
    if (!stream.eof())
        return false;
    result = value;
    return true;
}

template <typename T>
bool ReadNumberFromStream(std::istream& stream, T& result, T minValue, T maxValue)
{
    std::string line;
    T value;
    if (!ReadLineFromStream(stream, line) || !TryParseNumber(line, value) || value < minValue || value > maxValue)
        return false;
    result = value;
    return true;
}
