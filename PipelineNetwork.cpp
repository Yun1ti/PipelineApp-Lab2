#include "PipelineNetwork.h"

#include <fstream>

namespace
{
    const std::string PIPE_TAG = "PIPE";
    const std::string STATION_TAG = "STATION";

    template <typename T>
    void SaveRegistry(std::ostream& stream, const ObjectRegistry<T>& registry, const std::string& tag)
    {
        for (const auto& [id, object] : registry.GetAll())
        {
            stream << tag << "\n";
            object.SaveTo(stream);
        }
    }

    template <typename T>
    bool LoadObject(std::istream& stream, ObjectRegistry<T>& registry)
    {
        T object;
        return T::LoadFrom(stream, object) && registry.Insert(object);
    }
}

ObjectRegistry<Pipe>& PipelineNetwork::Pipes()
{
    return pipes;
}

const ObjectRegistry<Pipe>& PipelineNetwork::Pipes() const
{
    return pipes;
}

ObjectRegistry<CompressorStation>& PipelineNetwork::Stations()
{
    return stations;
}

const ObjectRegistry<CompressorStation>& PipelineNetwork::Stations() const
{
    return stations;
}

bool PipelineNetwork::IsEmpty() const
{
    return pipes.IsEmpty() && stations.IsEmpty();
}

bool PipelineNetwork::SaveToFile(const std::string& fileName) const
{
    std::ofstream file(fileName);
    if (!file)
        return false;
    SaveRegistry(file, pipes, PIPE_TAG);
    SaveRegistry(file, stations, STATION_TAG);
    return static_cast<bool>(file);
}

LoadResult PipelineNetwork::LoadFromFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file)
        return LoadResult::CANNOT_OPEN;

    PipelineNetwork loadedNetwork;
    std::string tag;
    while (ReadLineFromStream(file, tag))
    {
        if (ContainsOnlySpaces(tag))
            continue;
        bool isLoaded = false;
        if (tag == PIPE_TAG)
            isLoaded = LoadObject(file, loadedNetwork.pipes);
        else if (tag == STATION_TAG)
            isLoaded = LoadObject(file, loadedNetwork.stations);
        if (!isLoaded)
            return LoadResult::CORRUPTED;
    }
    *this = std::move(loadedNetwork);
    return LoadResult::SUCCESS;
}
