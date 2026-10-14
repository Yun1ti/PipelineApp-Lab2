#include "ConsoleMenu.h"

#include <climits>

namespace
{
    enum MenuAction
    {
        EXIT = 0,
        ADD_PIPE,
        ADD_STATION,
        SHOW_ALL,
        EDIT_PIPE,
        EDIT_STATION,
        DELETE_PIPE,
        DELETE_STATION,
        SAVE,
        LOAD,
        LAST_ACTION = LOAD
    };

    template <typename T>
    void PrintAll(const ObjectRegistry<T>& registry, const std::string& emptyMessage)
    {
        if (registry.IsEmpty())
        {
            std::cout << emptyMessage << "\n";
            return;
        }
        for (const auto& [id, object] : registry.GetAll())
            std::cout << object << "\n";
    }
}

ConsoleMenu::ConsoleMenu(InputReader& inputReader) : reader(inputReader)
{
}

void ConsoleMenu::Run()
{
    try
    {
        do
        {
            PrintMainMenu();
        } while (ExecuteAction(reader.ReadNumber("Select action: ", static_cast<int>(EXIT), static_cast<int>(LAST_ACTION))));
    }
    catch (const InputClosedError&)
    {
        std::cout << "\nInput stream closed. Exiting.\n";
    }
}

void ConsoleMenu::PrintMainMenu() const
{
    std::cout << "\n===== Menu =====\n"
              << "1. Add pipe\n"
              << "2. Add compressor station\n"
              << "3. Show all objects\n"
              << "4. Edit pipe\n"
              << "5. Edit compressor station\n"
              << "6. Delete pipe\n"
              << "7. Delete compressor station\n"
              << "8. Save to file\n"
              << "9. Load from file\n"
              << "0. Exit\n";
}

bool ConsoleMenu::ExecuteAction(int action)
{
    switch (action)
    {
    case ADD_PIPE:
        AddPipe();
        break;
    case ADD_STATION:
        AddStation();
        break;
    case SHOW_ALL:
        ShowAllObjects();
        break;
    case EDIT_PIPE:
        EditPipe();
        break;
    case EDIT_STATION:
        EditStation();
        break;
    case DELETE_PIPE:
        DeletePipe();
        break;
    case DELETE_STATION:
        DeleteStation();
        break;
    case SAVE:
        SaveToFile();
        break;
    case LOAD:
        LoadFromFile();
        break;
    case EXIT:
        return false;
    }
    return true;
}

void ConsoleMenu::AddPipe()
{
    Pipe& pipe = network.Pipes().Create();
    pipe.ReadFromConsole(reader);
    std::cout << "Added: " << pipe << "\n";
}

void ConsoleMenu::AddStation()
{
    CompressorStation& station = network.Stations().Create();
    station.ReadFromConsole(reader);
    std::cout << "Added: " << station << "\n";
}

void ConsoleMenu::ShowAllObjects() const
{
    std::cout << "--- Pipes ---\n";
    PrintAll(network.Pipes(), "No pipes.");
    std::cout << "--- Compressor stations ---\n";
    PrintAll(network.Stations(), "No compressor stations.");
}

void ConsoleMenu::EditPipe()
{
    std::optional<int> id = SelectExistingId(network.Pipes(), "pipe");
    if (!id)
        return;
    Pipe& pipe = *network.Pipes().Find(*id);
    std::cout << pipe << "\n";
    pipe.SetUnderRepair(reader.ReadYesNo("Set the pipe under repair?"));
    std::cout << "Updated: " << pipe << "\n";
}

void ConsoleMenu::EditStation()
{
    std::optional<int> id = SelectExistingId(network.Stations(), "compressor station");
    if (!id)
        return;
    CompressorStation& station = *network.Stations().Find(*id);
    std::cout << station << "\n"
              << "1. Start workshop\n"
              << "2. Stop workshop\n"
              << "0. Back\n";
    switch (reader.ReadNumber("Select action: ", 0, 2))
    {
    case 1:
        std::cout << (station.StartWorkshop() ? "Workshop started.\n" : "Error: all workshops are already working.\n");
        break;
    case 2:
        std::cout << (station.StopWorkshop() ? "Workshop stopped.\n" : "Error: no working workshops to stop.\n");
        break;
    default:
        return;
    }
    std::cout << station << "\n";
}

void ConsoleMenu::DeletePipe()
{
    DeleteObject(network.Pipes(), "pipe");
}

void ConsoleMenu::DeleteStation()
{
    DeleteObject(network.Stations(), "compressor station");
}

void ConsoleMenu::SaveToFile()
{
    if (network.IsEmpty())
    {
        std::cout << "Error: nothing to save.\n";
        return;
    }
    std::string fileName = reader.ReadNonEmptyLine("File name: ");
    if (network.SaveToFile(fileName))
        std::cout << "Data saved to " << fileName << ".\n";
    else
        std::cout << "Error: cannot write file " << fileName << ".\n";
}

void ConsoleMenu::LoadFromFile()
{
    if (!network.IsEmpty() && !reader.ReadYesNo("Current data will be replaced. Continue?"))
        return;
    std::string fileName = reader.ReadNonEmptyLine("File name: ");
    switch (network.LoadFromFile(fileName))
    {
    case LoadResult::SUCCESS:
        std::cout << "Data loaded from " << fileName << ".\n";
        break;
    case LoadResult::CANNOT_OPEN:
        std::cout << "Error: cannot open file " << fileName << ".\n";
        break;
    case LoadResult::CORRUPTED:
        std::cout << "Error: file " << fileName << " is corrupted. Current data was kept.\n";
        break;
    }
}

template <typename T>
std::optional<int> ConsoleMenu::SelectExistingId(const ObjectRegistry<T>& registry, const std::string& objectName)
{
    if (registry.IsEmpty())
    {
        std::cout << "Error: there is no " << objectName << " yet.\n";
        return std::nullopt;
    }
    PrintAll(registry, "");
    while (true)
    {
        int id = reader.ReadNumber("Enter " + objectName + " ID (0 - cancel): ", 0, INT_MAX);
        if (id == 0)
            return std::nullopt;
        if (registry.Contains(id))
            return id;
        std::cout << "Error: " << objectName << " with ID " << id << " not found.\n";
    }
}

template <typename T>
void ConsoleMenu::DeleteObject(ObjectRegistry<T>& registry, const std::string& objectName)
{
    std::optional<int> id = SelectExistingId(registry, objectName);
    if (id && registry.Remove(*id))
        std::cout << "Deleted " << objectName << " #" << *id << ".\n";
}
