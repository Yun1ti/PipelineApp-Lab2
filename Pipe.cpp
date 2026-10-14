#include "Pipe.h"

#include <climits>
#include <iomanip>

const double MIN_LENGTH_KM = 0.001;
const double MAX_LENGTH_KM = 10000.0;
const int MIN_DIAMETER_MM = 1;
const int MAX_DIAMETER_MM = 5000;

Pipe::Pipe(int pipeId) : id(pipeId)
{
}

int Pipe::GetId() const
{
    return id;
}

const std::string& Pipe::GetKilometerMark() const
{
    return kilometerMark;
}

bool Pipe::IsUnderRepair() const
{
    return underRepair;
}

void Pipe::SetUnderRepair(bool isUnderRepair)
{
    underRepair = isUnderRepair;
}

void Pipe::ReadFromConsole(InputReader& reader)
{
    kilometerMark = reader.ReadNonEmptyLine("Kilometer mark (name): ");
    lengthKm = reader.ReadNumber("Length, km: ", MIN_LENGTH_KM, MAX_LENGTH_KM);
    diameterMm = reader.ReadNumber("Diameter, mm: ", MIN_DIAMETER_MM, MAX_DIAMETER_MM);
    underRepair = reader.ReadYesNo("Is the pipe under repair?");
}

void Pipe::SaveTo(std::ostream& stream) const
{
    stream << std::setprecision(10)
           << id << "\n"
           << kilometerMark << "\n"
           << lengthKm << "\n"
           << diameterMm << "\n"
           << underRepair << "\n";
}

bool Pipe::LoadFrom(std::istream& stream, Pipe& pipe)
{
    Pipe loadedPipe;
    int repairFlag = 0;
    if (!ReadNumberFromStream(stream, loadedPipe.id, 1, INT_MAX)
        || !ReadNonEmptyLineFromStream(stream, loadedPipe.kilometerMark)
        || !ReadNumberFromStream(stream, loadedPipe.lengthKm, MIN_LENGTH_KM, MAX_LENGTH_KM)
        || !ReadNumberFromStream(stream, loadedPipe.diameterMm, MIN_DIAMETER_MM, MAX_DIAMETER_MM)
        || !ReadNumberFromStream(stream, repairFlag, 0, 1))
        return false;
    loadedPipe.underRepair = repairFlag == 1;
    pipe = loadedPipe;
    return true;
}

std::ostream& operator<<(std::ostream& stream, const Pipe& pipe)
{
    return stream << "Pipe #" << pipe.id
                  << " | mark: " << pipe.kilometerMark
                  << " | length: " << pipe.lengthKm << " km"
                  << " | diameter: " << pipe.diameterMm << " mm"
                  << " | " << (pipe.underRepair ? "under repair" : "in operation");
}
