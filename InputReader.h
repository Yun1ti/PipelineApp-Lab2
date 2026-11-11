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

// Reads user input line by line.
// In interactive mode every entered line is written to the log file as is.
// In replay mode (input from a log file) every line is echoed to the console
// so the replayed session looks like it was typed by the user.
class InputReader
{
public:
    InputReader(std::istream& input, bool echoInput);

    bool StartLogging(const std::string& logFileName);
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
    bool echoInput;
    std::ofstream log;
};
