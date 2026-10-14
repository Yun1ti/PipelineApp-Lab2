#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "StreamUtils.h"

class InputClosedError : public std::runtime_error
{
public:
    InputClosedError() : std::runtime_error("Input stream closed") {}
};

class InputReader
{
public:
    InputReader(std::istream& input, const std::string& logFileName);

    bool IsLogging() const;
    std::string ReadLine();
    std::string ReadNonEmptyLine(const std::string& prompt);
    bool ReadYesNo(const std::string& prompt);
    std::vector<int> ReadIntList(const std::string& prompt);

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

private:
    std::istream& input;
    std::ofstream log;
};
