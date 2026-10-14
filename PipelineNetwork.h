#pragma once

#include "CompressorStation.h"
#include "ObjectRegistry.h"
#include "Pipe.h"

class PipelineNetwork
{
public:
    ObjectRegistry<Pipe>& Pipes();
    const ObjectRegistry<Pipe>& Pipes() const;
    ObjectRegistry<CompressorStation>& Stations();
    const ObjectRegistry<CompressorStation>& Stations() const;
    bool IsEmpty() const;

private:
    ObjectRegistry<Pipe> pipes;
    ObjectRegistry<CompressorStation> stations;
};
