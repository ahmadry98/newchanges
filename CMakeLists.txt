cmake_minimum_required(VERSION 3.23)
project(hw2yousef)

set(CMAKE_CXX_STANDARD 20)

add_executable(hw2yousef main.cpp MySimulator.cpp CleaningAlgorithm.cpp Hoover.cpp House.cpp Sensors.cpp MySimulator.cpp CleaningAlgorithm.cpp config.h abstract_algorithm.h)
