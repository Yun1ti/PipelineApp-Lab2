#include <fstream>
#include <iostream>
#include <optional>

#include "CompressorStation.h"
#include "InputUtils.h"
#include "Pipe.h"

const std::string DATA_FILE_NAME = "pipeline_data.txt";
const std::string PIPE_SECTION = "PIPE";
const std::string STATION_SECTION = "STATION";

enum MenuAction
{
    EXIT = 0,
    ADD_PIPE,
    ADD_STATION,
    SHOW_ALL,
    EDIT_PIPE,
    EDIT_STATION,
    SAVE,
    LOAD,
    LAST_ACTION = LOAD
};

void PrintMenu()
{
    std::cout << "\n===== Menu =====\n"
              << "1. Add pipe\n"
              << "2. Add compressor station\n"
              << "3. Show all objects\n"
              << "4. Edit pipe\n"
              << "5. Edit compressor station\n"
              << "6. Save to file\n"
              << "7. Load from file\n"
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

void SaveData(const std::optional<Pipe>& pipe, const std::optional<CompressorStation>& station)
{
    if (!pipe && !station)
    {
        std::cout << "Error: nothing to save.\n";
        return;
    }
    std::ofstream file(DATA_FILE_NAME);
    if (!file)
    {
        std::cout << "Error: cannot open file " << DATA_FILE_NAME << " for writing.\n";
        return;
    }
    if (pipe)
    {
        file << PIPE_SECTION << "\n";
        pipe->SaveToStream(file);
    }
    if (station)
    {
        file << STATION_SECTION << "\n";
        station->SaveToStream(file);
    }
    std::cout << (file ? "Data saved to " + DATA_FILE_NAME + ".\n" : "Error: failed to write data.\n");
}

void LoadData(std::optional<Pipe>& pipe, std::optional<CompressorStation>& station)
{
    std::ifstream file(DATA_FILE_NAME);
    if (!file)
    {
        std::cout << "Error: cannot open file " << DATA_FILE_NAME << ".\n";
        return;
    }

    std::optional<Pipe> loadedPipe;
    std::optional<CompressorStation> loadedStation;
    std::string sectionName;
    while (ReadLineFromStream(file, sectionName))
    {
        if (sectionName.empty())
            continue;
        if (sectionName == PIPE_SECTION && !loadedPipe)
        {
            Pipe newPipe;
            if (!newPipe.LoadFromStream(file))
            {
                std::cout << "Error: pipe data in file is corrupted. Nothing loaded.\n";
                return;
            }
            loadedPipe = newPipe;
        }
        else if (sectionName == STATION_SECTION && !loadedStation)
        {
            CompressorStation newStation;
            if (!newStation.LoadFromStream(file))
            {
                std::cout << "Error: compressor station data in file is corrupted. Nothing loaded.\n";
                return;
            }
            loadedStation = newStation;
        }
        else
        {
            std::cout << "Error: unexpected data in file. Nothing loaded.\n";
            return;
        }
    }

    if (!loadedPipe && !loadedStation)
    {
        std::cout << "Error: file contains no objects.\n";
        return;
    }
    pipe = loadedPipe;
    station = loadedStation;
    std::cout << "Data loaded from " << DATA_FILE_NAME << ".\n";
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
            case SAVE:
                SaveData(pipe, station);
                break;
            case LOAD:
                LoadData(pipe, station);
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
