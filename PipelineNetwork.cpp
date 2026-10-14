#include "PipelineNetwork.h"

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
