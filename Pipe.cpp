#include "Pipe.h"
#include "InputUtils.h"

#include <iomanip>

const double MIN_LENGTH_KM = 0.001;
const double MAX_LENGTH_KM = 10000.0;
const int MIN_DIAMETER_MM = 1;
const int MAX_DIAMETER_MM = 5000;

void Pipe::ReadFromConsole()
{
    kilometerMark = ReadNonEmptyLine("Kilometer mark (name): ");
    lengthKm = ReadNumber("Length, km: ", MIN_LENGTH_KM, MAX_LENGTH_KM);
    diameterMm = ReadNumber("Diameter, mm: ", MIN_DIAMETER_MM, MAX_DIAMETER_MM);
    underRepair = ReadYesNo("Is the pipe under repair?");
}

void Pipe::PrintToConsole() const
{
    std::cout << "Pipe\n"
              << "  Kilometer mark: " << kilometerMark << "\n"
              << "  Length:         " << lengthKm << " km\n"
              << "  Diameter:       " << diameterMm << " mm\n"
              << "  Status:         " << (underRepair ? "under repair" : "in operation") << "\n";
}

void Pipe::SetUnderRepair(bool isUnderRepair)
{
    underRepair = isUnderRepair;
}

bool Pipe::IsUnderRepair() const
{
    return underRepair;
}

void Pipe::SaveToStream(std::ostream& stream) const
{
    stream << std::setprecision(10) << kilometerMark << "\n"
           << lengthKm << "\n"
           << diameterMm << "\n"
           << underRepair << "\n";
}

bool Pipe::LoadFromStream(std::istream& stream)
{
    Pipe loadedPipe;
    int repairFlag = 0;
    if (!ReadNonEmptyLineFromStream(stream, loadedPipe.kilometerMark)
        || !ReadNumberFromStream(stream, loadedPipe.lengthKm, MIN_LENGTH_KM, MAX_LENGTH_KM)
        || !ReadNumberFromStream(stream, loadedPipe.diameterMm, MIN_DIAMETER_MM, MAX_DIAMETER_MM)
        || !ReadNumberFromStream(stream, repairFlag, 0, 1))
        return false;
    loadedPipe.underRepair = repairFlag == 1;
    *this = loadedPipe;
    return true;
}
