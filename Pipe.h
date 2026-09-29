#pragma once

#include <string>

class Pipe
{
public:
    void ReadFromConsole();
    void PrintToConsole() const;
    void SetUnderRepair(bool underRepair);
    bool IsUnderRepair() const;

private:
    std::string kilometerMark;
    double lengthKm = 0.0;
    int diameterMm = 0;
    bool underRepair = false;
};
