#include "ConsoleMenu.h"

#include <algorithm>
#include <cctype>
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
        SEARCH_PIPES,
        SEARCH_STATIONS,
        SAVE,
        LOAD,
        LAST_ACTION = LOAD
    };

    enum BatchAction
    {
        BATCH_BACK = 0,
        EDIT_ALL_FOUND,
        EDIT_SELECTED,
        DELETE_ALL_FOUND,
        DELETE_SELECTED,
        LAST_BATCH_ACTION = DELETE_SELECTED
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

    template <typename T>
    void PrintFound(const ObjectRegistry<T>& registry, const std::vector<int>& ids)
    {
        std::cout << "Found: " << ids.size() << "\n";
        for (int id : ids)
            std::cout << registry.GetAll().at(id) << "\n";
    }

    std::string ToLower(std::string text)
    {
        std::transform(text.begin(), text.end(), text.begin(),
                       [](unsigned char symbol) { return static_cast<char>(std::tolower(symbol)); });
        return text;
    }

    bool ContainsIgnoreCase(const std::string& text, const std::string& pattern)
    {
        return ToLower(text).find(ToLower(pattern)) != std::string::npos;
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
              << "8. Search pipes (batch editing)\n"
              << "9. Search compressor stations\n"
              << "10. Save to file\n"
              << "11. Load from file\n"
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
    case SEARCH_PIPES:
        SearchPipes();
        break;
    case SEARCH_STATIONS:
        SearchStations();
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

std::vector<int> ConsoleMenu::FindPipes()
{
    std::cout << "1. By kilometer mark (name)\n"
              << "2. By repair status\n"
              << "3. All pipes\n";
    switch (reader.ReadNumber("Select filter: ", 1, 3))
    {
    case 1:
    {
        std::string pattern = reader.ReadNonEmptyLine("Name contains: ");
        return network.Pipes().FindIds([&pattern](const Pipe& pipe)
                                       { return ContainsIgnoreCase(pipe.GetKilometerMark(), pattern); });
    }
    case 2:
    {
        bool underRepair = reader.ReadYesNo("Find pipes under repair?");
        return network.Pipes().FindIds([underRepair](const Pipe& pipe)
                                       { return pipe.IsUnderRepair() == underRepair; });
    }
    default:
        return network.Pipes().FindIds([](const Pipe&) { return true; });
    }
}

std::vector<int> ConsoleMenu::FindStations()
{
    std::cout << "1. By name\n"
              << "2. By percent of idle workshops\n";
    if (reader.ReadNumber("Select filter: ", 1, 2) == 1)
    {
        std::string pattern = reader.ReadNonEmptyLine("Name contains: ");
        return network.Stations().FindIds([&pattern](const CompressorStation& station)
                                          { return ContainsIgnoreCase(station.GetName(), pattern); });
    }
    double minPercent = reader.ReadNumber("Minimum idle percent (0-100): ", 0.0, 100.0);
    double maxPercent = reader.ReadNumber("Maximum idle percent: ", minPercent, 100.0);
    return network.Stations().FindIds([minPercent, maxPercent](const CompressorStation& station)
                                      {
                                          double idlePercent = station.GetIdleWorkshopPercent();
                                          return idlePercent >= minPercent && idlePercent <= maxPercent;
                                      });
}

void ConsoleMenu::SearchPipes()
{
    if (network.Pipes().IsEmpty())
    {
        std::cout << "Error: there is no pipe yet.\n";
        return;
    }
    std::vector<int> foundIds = FindPipes();
    PrintFound(network.Pipes(), foundIds);
    if (!foundIds.empty())
        BatchEditPipes(foundIds);
}

void ConsoleMenu::BatchEditPipes(const std::vector<int>& foundIds)
{
    std::cout << "1. Edit repair status of all found pipes\n"
              << "2. Edit repair status of selected pipes\n"
              << "3. Delete all found pipes\n"
              << "4. Delete selected pipes\n"
              << "0. Back\n";
    int action = reader.ReadNumber("Select action: ", static_cast<int>(BATCH_BACK), static_cast<int>(LAST_BATCH_ACTION));
    if (action == BATCH_BACK)
        return;

    bool isForAllFound = action == EDIT_ALL_FOUND || action == DELETE_ALL_FOUND;
    std::vector<int> targetIds = isForAllFound ? foundIds : SelectPipeIds(foundIds);
    if (action == EDIT_ALL_FOUND || action == EDIT_SELECTED)
        SetPipesRepairStatus(targetIds);
    else
        DeletePipes(targetIds);
}

std::vector<int> ConsoleMenu::SelectPipeIds(const std::vector<int>& allowedIds)
{
    while (true)
    {
        std::vector<int> ids = reader.ReadIntList("Enter pipe IDs separated by spaces: ");
        auto notAllowed = std::find_if(ids.begin(), ids.end(), [&allowedIds](int id)
                                       { return !std::binary_search(allowedIds.begin(), allowedIds.end(), id); });
        if (notAllowed == ids.end())
        {
            std::sort(ids.begin(), ids.end());
            ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
            return ids;
        }
        std::cout << "Error: pipe #" << *notAllowed << " is not among the found pipes.\n";
    }
}

void ConsoleMenu::SetPipesRepairStatus(const std::vector<int>& ids)
{
    bool underRepair = reader.ReadYesNo("Set these pipes under repair?");
    for (int id : ids)
        network.Pipes().Find(id)->SetUnderRepair(underRepair);
    std::cout << "Updated pipes: " << ids.size() << "\n";
}

void ConsoleMenu::DeletePipes(const std::vector<int>& ids)
{
    if (!reader.ReadYesNo("Delete " + std::to_string(ids.size()) + " pipe(s)?"))
        return;
    for (int id : ids)
        network.Pipes().Remove(id);
    std::cout << "Deleted pipes: " << ids.size() << "\n";
}

void ConsoleMenu::SearchStations()
{
    if (network.Stations().IsEmpty())
    {
        std::cout << "Error: there is no compressor station yet.\n";
        return;
    }
    PrintFound(network.Stations(), FindStations());
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
