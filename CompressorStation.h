#pragma once

#include <iostream>
#include <string>

#include "InputReader.h"

class CompressorStation
{
public:
    explicit CompressorStation(int id = 0);

    int GetId() const;
    const std::string& GetName() const;
    double GetIdleWorkshopPercent() const;
    bool StartWorkshop();
    bool StopWorkshop();

    void ReadFromConsole(InputReader& reader);
    void SaveTo(std::ostream& stream) const;
    static bool LoadFrom(std::istream& stream, CompressorStation& station);

    friend std::ostream& operator<<(std::ostream& stream, const CompressorStation& station);

private:
    int id;
    std::string name;
    int workshopCount = 0;
    int workingWorkshopCount = 0;
    int stationClass = 1;
};
