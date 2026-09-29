#pragma once

#include <iostream>
#include <string>

class CompressorStation
{
public:
    void ReadFromConsole();
    void PrintToConsole() const;
    bool StartWorkshop();
    bool StopWorkshop();
    void SaveToStream(std::ostream& stream) const;
    bool LoadFromStream(std::istream& stream);

private:
    std::string name;
    int workshopCount = 0;
    int workingWorkshopCount = 0;
    int stationClass = 1;
};
