#pragma once

#include <iostream>
#include <string>

#include "InputReader.h"

class Pipe
{
public:
    explicit Pipe(int id = 0);

    int GetId() const;
    const std::string& GetKilometerMark() const;
    bool IsUnderRepair() const;
    void SetUnderRepair(bool underRepair);

    void ReadFromConsole(InputReader& reader);
    void SaveTo(std::ostream& stream) const;
    static bool LoadFrom(std::istream& stream, Pipe& pipe);

    friend std::ostream& operator<<(std::ostream& stream, const Pipe& pipe);

private:
    int id;
    std::string kilometerMark;
    double lengthKm = 0.0;
    int diameterMm = 0;
    bool underRepair = false;
};
