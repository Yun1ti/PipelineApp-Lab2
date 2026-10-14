#include "CompressorStation.h"

#include <climits>

const int MAX_WORKSHOP_COUNT = 100;
const int MIN_STATION_CLASS = 1;
const int MAX_STATION_CLASS = 5;

CompressorStation::CompressorStation(int stationId) : id(stationId)
{
}

int CompressorStation::GetId() const
{
    return id;
}

const std::string& CompressorStation::GetName() const
{
    return name;
}

double CompressorStation::GetIdleWorkshopPercent() const
{
    return 100.0 * (workshopCount - workingWorkshopCount) / workshopCount;
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

void CompressorStation::ReadFromConsole(InputReader& reader)
{
    name = reader.ReadNonEmptyLine("Station name: ");
    workshopCount = reader.ReadNumber("Number of workshops: ", 1, MAX_WORKSHOP_COUNT);
    workingWorkshopCount = reader.ReadNumber("Number of working workshops: ", 0, workshopCount);
    stationClass = reader.ReadNumber("Station class (1-5): ", MIN_STATION_CLASS, MAX_STATION_CLASS);
}

void CompressorStation::SaveTo(std::ostream& stream) const
{
    stream << id << "\n"
           << name << "\n"
           << workshopCount << "\n"
           << workingWorkshopCount << "\n"
           << stationClass << "\n";
}

bool CompressorStation::LoadFrom(std::istream& stream, CompressorStation& station)
{
    CompressorStation loadedStation;
    if (!ReadNumberFromStream(stream, loadedStation.id, 1, INT_MAX)
        || !ReadNonEmptyLineFromStream(stream, loadedStation.name)
        || !ReadNumberFromStream(stream, loadedStation.workshopCount, 1, MAX_WORKSHOP_COUNT)
        || !ReadNumberFromStream(stream, loadedStation.workingWorkshopCount, 0, loadedStation.workshopCount)
        || !ReadNumberFromStream(stream, loadedStation.stationClass, MIN_STATION_CLASS, MAX_STATION_CLASS))
        return false;
    station = loadedStation;
    return true;
}

std::ostream& operator<<(std::ostream& stream, const CompressorStation& station)
{
    return stream << "Station #" << station.id
                  << " | name: " << station.name
                  << " | workshops: " << station.workingWorkshopCount << "/" << station.workshopCount << " working"
                  << " | idle: " << station.GetIdleWorkshopPercent() << "%"
                  << " | class: " << station.stationClass;
}
