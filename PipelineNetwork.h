#pragma once

#include "CompressorStation.h"
#include "ObjectRegistry.h"
#include "Pipe.h"

#include <string>

enum class LoadResult
{
    SUCCESS,
    CANNOT_OPEN,
    CORRUPTED
};

class PipelineNetwork
{
public:
    ObjectRegistry<Pipe>& Pipes();
    const ObjectRegistry<Pipe>& Pipes() const;
    ObjectRegistry<CompressorStation>& Stations();
    const ObjectRegistry<CompressorStation>& Stations() const;
    bool IsEmpty() const;
    bool SaveToFile(const std::string& fileName) const;
    LoadResult LoadFromFile(const std::string& fileName);

private:
    ObjectRegistry<Pipe> pipes;
    ObjectRegistry<CompressorStation> stations;
};
