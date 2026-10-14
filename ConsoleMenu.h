#pragma once

#include <optional>
#include <string>
#include <vector>

#include "InputReader.h"
#include "PipelineNetwork.h"

class ConsoleMenu
{
public:
    explicit ConsoleMenu(InputReader& reader);
    void Run();

private:
    InputReader& reader;
    PipelineNetwork network;

    void PrintMainMenu() const;
    bool ExecuteAction(int action);

    void AddPipe();
    void AddStation();
    void ShowAllObjects() const;
    void EditPipe();
    void EditStation();
    void DeletePipe();
    void DeleteStation();
    void SearchPipes();
    void SearchStations();
    std::vector<int> FindPipes();
    std::vector<int> FindStations();
    void BatchEditPipes(const std::vector<int>& foundIds);
    std::vector<int> SelectPipeIds(const std::vector<int>& allowedIds);
    void SetPipesRepairStatus(const std::vector<int>& ids);
    void DeletePipes(const std::vector<int>& ids);
    void SaveToFile();
    void LoadFromFile();

    template <typename T>
    std::optional<int> SelectExistingId(const ObjectRegistry<T>& registry, const std::string& objectName);

    template <typename T>
    void DeleteObject(ObjectRegistry<T>& registry, const std::string& objectName);
};
