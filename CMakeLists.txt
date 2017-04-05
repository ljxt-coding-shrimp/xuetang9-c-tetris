cmake_minimum_required(VERSION 3.6)
project(tetris)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES contorl/main.c common/WinApiTool.c  model/Shap.c  contorl/Contorl.c view/Draw.c model/Map.c)
add_executable(tetris ${SOURCE_FILES})