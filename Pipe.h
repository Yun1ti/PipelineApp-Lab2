#pragma once

#include <iostream>
#include <string>

class Pipe
{
public:
    void ReadFromConsole();
    void PrintToConsole() const;
    void SetUnderRepair(bool underRepair);
    bool IsUnderRepair() const;
    void SaveToStream(std::ostream& stream) const;
    bool LoadFromStream(std::istream& stream);

private:
    std::string kilometerMark;
    double lengthKm = 0.0;
    int diameterMm = 0;
    bool underRepair = false;
};
