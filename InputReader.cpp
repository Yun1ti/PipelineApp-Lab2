#include "InputReader.h"

InputReader::InputReader(std::istream& inputStream, const std::string& logFileName)
    : input(inputStream), log(logFileName)
{
}

bool InputReader::IsLogging() const
{
    return log.is_open();
}

std::string InputReader::ReadLine()
{
    std::string line;
    if (!ReadLineFromStream(input, line))
        throw InputClosedError();
    if (log)
        log << line << std::endl;
    return line;
}

std::string InputReader::ReadNonEmptyLine(const std::string& prompt)
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

bool InputReader::ReadYesNo(const std::string& prompt)
{
    return ReadNumber(prompt + " (1 - yes, 0 - no): ", 0, 1) == 1;
}

std::vector<int> InputReader::ReadIntList(const std::string& prompt)
{
    while (true)
    {
        std::cout << prompt;
        std::istringstream stream(ReadLine());
        std::vector<int> numbers;
        int number;
        while (stream >> number)
            numbers.push_back(number);
        if (stream.eof() && !numbers.empty())
            return numbers;
        std::cout << "Error: enter one or more integers separated by spaces.\n";
    }
}
