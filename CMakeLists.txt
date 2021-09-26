cmake_minimum_required(VERSION 3.10)

project(MainSolution)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC src/*.cpp src/*.h src/*.hpp)

add_executable(VectorAndMatrix ${SRC})

message("***** "  ${PROJECT_NAME}  " ***** "  ${SRC}  " *****")