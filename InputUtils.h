#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class InputClosedError : public std::runtime_error
{
public:
    InputClosedError() : std::runtime_error("Input stream closed") {}
};

std::string ReadLine();
std::string ReadNonEmptyLine(const std::string& prompt);
bool ReadYesNo(const std::string& prompt);
bool ReadLineFromStream(std::istream& stream, std::string& line);
bool ReadNonEmptyLineFromStream(std::istream& stream, std::string& line);

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
T ReadNumber(const std::string& prompt, T minValue, T maxValue)
{
    while (true)
    {
        std::cout << prompt;
        T value;
        if (TryParseNumber(ReadLine(), value) && value >= minValue && value <= maxValue)
            return value;
        std::cout << "Error: enter a number from " << minValue << " to " << maxValue << ".\n";
    }
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
