cmake_minimum_required(VERSION 3.10)
project(PipelineApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PipelineApp
    main.cpp
    InputUtils.cpp
    StreamUtils.cpp
    InputReader.cpp
    Pipe.cpp
    CompressorStation.cpp
)
