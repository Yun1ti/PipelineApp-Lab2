#include "CompressorStation.h"
#include "InputUtils.h"

const int MAX_WORKSHOP_COUNT = 100;
const int MIN_STATION_CLASS = 1;
const int MAX_STATION_CLASS = 5;

void CompressorStation::ReadFromConsole()
{
    name = ReadNonEmptyLine("Station name: ");
    workshopCount = ReadNumber("Number of workshops: ", 1, MAX_WORKSHOP_COUNT);
    workingWorkshopCount = ReadNumber("Number of working workshops: ", 0, workshopCount);
    stationClass = ReadNumber("Station class (1-5): ", MIN_STATION_CLASS, MAX_STATION_CLASS);
}

void CompressorStation::PrintToConsole() const
{
    std::cout << "Compressor station\n"
              << "  Name:              " << name << "\n"
              << "  Workshops:         " << workshopCount << "\n"
              << "  Working workshops: " << workingWorkshopCount << "\n"
              << "  Station class:     " << stationClass << "\n";
}

bool CompressorStation::StartWorkshop()
{
    if (workingWorkshopCount >= workshopCount)
        return false;
    ++workingWorkshopCount;
    return true;
}

bool CompressorStation::StopWorkshop()
{
    if (workingWorkshopCount <= 0)
        return false;
    --workingWorkshopCount;
    return true;
}
