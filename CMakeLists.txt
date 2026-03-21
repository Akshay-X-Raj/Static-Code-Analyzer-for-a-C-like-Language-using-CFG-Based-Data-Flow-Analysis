cmake_minimum_required(VERSION 3.10)
project(StaticAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/parser src/ast src/semantic src/cfg src/dataflow src/viz src/utils)

file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
)

add_executable(static_analyzer ${SRC_FILES})
