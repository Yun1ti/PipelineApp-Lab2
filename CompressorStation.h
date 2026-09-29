#pragma once

#include <string>

class CompressorStation
{
public:
    void ReadFromConsole();
    void PrintToConsole() const;
    bool StartWorkshop();
    bool StopWorkshop();

private:
    std::string name;
    int workshopCount = 0;
    int workingWorkshopCount = 0;
    int stationClass = 1;
};
