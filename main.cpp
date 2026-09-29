#include <iostream>
#include <optional>

#include "CompressorStation.h"
#include "InputUtils.h"
#include "Pipe.h"

enum MenuAction
{
    EXIT = 0,
    ADD_PIPE,
    ADD_STATION,
    SHOW_ALL,
    EDIT_PIPE,
    EDIT_STATION,
    LAST_ACTION = EDIT_STATION
};

void PrintMenu()
{
    std::cout << "\n===== Menu =====\n"
              << "1. Add pipe\n"
              << "2. Add compressor station\n"
              << "3. Show all objects\n"
              << "4. Edit pipe\n"
              << "5. Edit compressor station\n"
              << "0. Exit\n";
}

void AddPipe(std::optional<Pipe>& pipe)
{
    if (pipe && !ReadYesNo("Pipe already exists. Replace it?"))
        return;
    Pipe newPipe;
    newPipe.ReadFromConsole();
    pipe = newPipe;
    std::cout << "Pipe added.\n";
}

void AddStation(std::optional<CompressorStation>& station)
{
    if (station && !ReadYesNo("Compressor station already exists. Replace it?"))
        return;
    CompressorStation newStation;
    newStation.ReadFromConsole();
    station = newStation;
    std::cout << "Compressor station added.\n";
}

void ShowAllObjects(const std::optional<Pipe>& pipe, const std::optional<CompressorStation>& station)
{
    if (pipe)
        pipe->PrintToConsole();
    else
        std::cout << "Pipe has not been added.\n";

    if (station)
        station->PrintToConsole();
    else
        std::cout << "Compressor station has not been added.\n";
}

void EditPipe(std::optional<Pipe>& pipe)
{
    if (!pipe)
    {
        std::cout << "Error: add a pipe first.\n";
        return;
    }
    std::cout << "Current status: " << (pipe->IsUnderRepair() ? "under repair" : "in operation") << "\n";
    pipe->SetUnderRepair(ReadYesNo("Set the pipe under repair?"));
    std::cout << "Pipe status updated.\n";
}

void EditStation(std::optional<CompressorStation>& station)
{
    if (!station)
    {
        std::cout << "Error: add a compressor station first.\n";
        return;
    }
    std::cout << "1. Start workshop\n"
              << "2. Stop workshop\n"
              << "0. Back\n";
    switch (ReadNumber("Select action: ", 0, 2))
    {
    case 1:
        std::cout << (station->StartWorkshop() ? "Workshop started.\n" : "Error: all workshops are already working.\n");
        break;
    case 2:
        std::cout << (station->StopWorkshop() ? "Workshop stopped.\n" : "Error: no working workshops to stop.\n");
        break;
    default:
        break;
    }
}

int main()
{
    std::optional<Pipe> pipe;
    std::optional<CompressorStation> station;

    try
    {
        while (true)
        {
            PrintMenu();
            switch (ReadNumber("Select action: ", static_cast<int>(EXIT), static_cast<int>(LAST_ACTION)))
            {
            case ADD_PIPE:
                AddPipe(pipe);
                break;
            case ADD_STATION:
                AddStation(station);
                break;
            case SHOW_ALL:
                ShowAllObjects(pipe, station);
                break;
            case EDIT_PIPE:
                EditPipe(pipe);
                break;
            case EDIT_STATION:
                EditStation(station);
                break;
            case EXIT:
                return 0;
            }
        }
    }
    catch (const InputClosedError&)
    {
        std::cout << "\nInput stream closed. Exiting.\n";
    }
    return 0;
}
